############
Architecture
############

LwMQ messaging's architecture is DMA-first. What that means is
the messaging framework revolves around the high-level concept
of a memory window being "teleported" from one peer's address
space to another.

The actual data transfer happens either through directly shared
memory (for the IPC transport) or with the help of the network
adapter for the RDMA case, where the transfer is offloaded to
the network hardware.

We are investigating options to enable cross-VM
and cross-container shared memory support in the future to enable
direct VM-to-VM (or container) communication, but this requires a
little help from the hypervisor.

Other transports, such as Hyper-V's HvSocket, rely on an existing
pipe-like infrastructure, while yet other transports, like
a possible TCP transport, rely on pinned memory windows and queues
that sit in-between user-mode and kernel-mode to minimize buffer
copies and kernel transitions.

General Architecture
====================

LwMQ's is roughly divided in two main distinct layers: the transport
layer and the message queuing layer.

Transport Layer
---------------

Each transport exposes a well-defined function table conforming to
the LwMQ TLI (transport layer interface).

Transport discovery is performed at runtime. The transport layer does
not know anything about specific transports but discovers them
dynamically by examining a custom PE section, .lmqini, where each
transports registers its entry point at compile time.

The scheme provide full logical separation between transports and
the transport layer and allows for future expansion.

Adding a new transport boils down to adding the new transport's
entry point to the transport's table in the .lmqini section,
something done automatically through some linker magic.

Transports expose capabilities, such as the ability to send, receive,
multicast, etc, as well as address parsing. Upper layers atop the
TLI don't know much about transports either. The matching is done
mainly by the URI scheme, for example "ipc://" is passed to each
transport until one of them handles it.


The address parsing is then performed by the selected transport: LwMQ
does not know anything about IP addresses, paths, reserved characters,
etc, as the parsing is performed by the transport themselves.

The LwMQ API has provisions for :ref:`messaging-direct-buffer-access`. It is therefore
possible to build any upper-layer leveraging LwMQ's buffer teleportation
to meet any special requirements not covered by message queuing,
for example bulk BLOB transfer of any size or other scenarios where
discrete messages are not ideal.

The transport layer operates asynchronously, in the sense that an
upper layer can obtain a buffer, fill it, and send it, then immediately
try obtaining another buffer, which will succeed provided that a buffer
is available, or timeout. The buffer queue length is under application
control and the TLI only blocks when no buffer is immediately available.

We don't provide guidance regarding the number of send and/or receive
buffers as those numbers are dependent on the workload, transport speed,
etc, but an application can conceivably have buffer long buffer queues with
dozens or even hundreds of buffers.

The built-in IPC transport has a limit of approximately 4,000 buffers
shared between both directions of traffic (e.g. you can have 3,500 buffers
in one direction and 500 in the other), with no practical per-buffer size
limit, and no practical limit on channel count.

Other transports can have very different limits. For example RDMA adapters
often have a maximum memory aperture size, e.g. 2GB, and sometimes limit
the total aperture size adapter-wide, or the count of memory regions, which
in turn affects the buffer size/count as well as the number of LwMQ channels
that can be established with whatever buffer size/count on that particular
adapter.

While LwMQ sounds like Christmas came earlier, we don't recommend to go
crazy with the buffer queue lengths and buffer sizes. Large buffers are
useful in direct scenarios when moving huge blobs, and also allow clubbing
messages together if the producer is very quick and queues messages faster
than the underlying transport can move them.

Long buffer queues can help absorb message traffic spikes but also expose
the application to more risks in case of crash as more messages will be
lost.

Both increase memory usage, so use sensible queue lengths and appropriate
buffer sizes to meet your requirements with minimal resource usage.


Future
^^^^^^

If the need arises, the scheme can easily be extended to discovery
of external transport modules in the future after v1, with the understanding
that external transports will have priority over the built-in ones:
if someone comes up with a better IPC transport, dropping an external
module handling the "ipc://" scheme, in the field, will override the
internal one without code change to LwMQ, and more importantly
without changes to application code.

It is also easy to imagine transport filters, which could be installed
to intercept transport activity for debugging, logging, or auditing
purposes.

In any case, LwMQ will only load EV-signed modules.

Message Queuing Layer
---------------------

The message queuing layer exposes various input and output queues
and handles the serialization / deserialization of messages and their
data frames to and from transport buffers, and takes care of buffer
processing.

The message queuing layer handles queue priorities and utilizes a
weighted round-robin message collection algorithm to gather messages
from various priority groups.

Each priority group can have any number of any type of input queue,
for example the NORMAL priority group on a given LwMQ channel can
have five mono-producer unbounded queue and a multi-producer bounded
queue, both accessed concurrency by various threads queuing messages
asynchronously.

In this example, the five threads bound to the five mono-producer
queues can post messages concurrently and unobstructed at an extremely
high rate (multi-millions per second per thread) while more threads
sharing the multi-producer queue are synchronized by the queue itself,
trading concurrency for the convenience of a shared queue.

The messaging layer impose very little to the application or systems
architect: the functionality exposed for messaging is flexible and
accommodate many scenarios.

Sender Block Diagram
====================

The diagram below shows a particular configuration where a channel
has three input queues as an example. The queues are connected to the
input Scheduler on their consumer end.

.. mermaid::

   ---
   title: LwMQ Sender Side (Simplified)
   config:
      theme: 'neutral'   
   ---

   graph
      A(Input Queue 1) --> F
      B(Input Queue 2) --> F
      C(Input Queue N) --> F(Input Scheduler)
      F --> G(Message Encoder)
      G --> H(Transport Buffer A)
      G --> I(Transport Buffer B)
      G --> J(Transport Buffer C)
      H --> L(Transport A)
      I --> M(Transport B)
      J --> N(Transport C)

The input scheduler is responsible for gathering messages from
queues according to the queue's priorities. The messages are
sent to the message encoder in different manners: messages
of various priorities can be interleaved and messages from
queues at the same priority are collected in round-robin fashion,
while messages from the same queue are always sent in the order
they were posted.

The message encoder produces the "wire format" that is laid down
to the transport buffers.

The transport buffers are then "shipped" to their respective
device for actual transport.

The scheduler never waits for potential messages to be posted
before shipping the buffers to the devices. It will club messages
together as they come, but it will ship the buffers as soon as
the buffers are full or the input queues are empty. The application
controls the buffer sizes and their count and can therefore tune
the system for the best possible performance for the scenario at hand.

A channel can support any number of input queues. Using separate
queues, as needed, help reduce contentions. Also, the priority
scheme is based on *queue priorities* rather than message
priorities so a common use would be to have a normal priority
input queue and one or more additional queues of different
or same priorities as needed. If an application has multiple
threads producing messages, it is advisable to use a queue
per sending thread to eliminate contentions.

In situations where the application does not control its threads,
for example when using a task or concurrency framework, coroutines,
and other contraptions, LwMQ supplies multi-producer queues that
implement internal synchronization.

A channel can support an arbitrary number of transports and even
multiple instances of the same transport, all of course subjected
to system limits and a bit of common sense.

When multiple transports are attached to a channel, the messages
are laid down to each transport buffer separately. This is necessary
as LwMQ often does not own the transport buffers as they belong to
the device. There is therefore a linear cost to encoding a message
to each transport buffer. The application can choose to attach as
many transports as it needs, but should be mindful of the cost of
encoding messages to multiple buffers.

Messages must fit in the transport buffers, so the application
must size the buffers accordingly. If a message is larger than
the transport buffer size, it is rejected by the message encoder.
This is the only way to ensure true atomic message delivery
as any particular message is either send and delivered in its
entirety or not at all.

LwMQ's channels do not maintain state related to messages: the
application creates messages independently of any channel or connection,
and adds data frames and content to the message as it sees fit.

The messages are only associated with a channel when the application
posts it to one of the channel's input queues. This solves many
issues that inherently prevent other messaging systems from
supporting true multithreaded operations and true atomic message
transport.

Not maintaining state at the channel level also solves
issues inherent to request-response patterns where an application
can post any number of concurrent requests without waiting for
an answer, while competing solutions often require the application
to wait for the response to a request before posting the next one,
which is a major bottleneck in concurrent scenarios and also causes
the sender to be stuck indefinitely if the receiver fails to respond
for any reason.

LwMQ solves those problems and more, while providing best-in-industry
performance and a simple API that can be learned and put to work
on day one.

Receiver Block Diagram
======================

On the receiving side, transports are serviced by threads that
collects messages from incoming transport buffers. The decoded
messages are then posted to the channel's output queue where
they wait for the application to retrieve them.

.. mermaid::

   ---
   title: LwMQ Receiver Side (Simplified)
   config:
      theme: 'neutral'
   ---

   graph
      A(Transport A) --> TA(Transport Buffer A)
      B(Transport B) --> TB(Transport Buffer B)
      C(Transport C) --> TC(Transport Buffer C)
      TA --> MDA(Message Decoder)
      TB --> MDB(Message Decoder)
      TC --> MDC(Message Decoder)
      MDA --> OQ(Output Queue)
      MDB --> OQ(Output Queue)
      MDC --> OQ(Output Queue)      
