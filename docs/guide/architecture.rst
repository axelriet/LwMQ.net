############
Architecture
############

LwMQ messaging's architecture is DMA-first. What that means is
the messaging framework revolves around the high-level concept
of a memory window being "teleported" from one peer's address
space to another.

The actual transport happens either through directly shared
memory (for the IPC transport) or with the help of the network
adapter for the RDMA case, where the transfer is offloaded to
the hardware.

Other transports, such as Hyper-V's HvSocket, rely on an existing
pipe-like infrastructure, while yet other transports, like
a possible TCP transport, rely on pinned memory windows and queues
that sit in-between user-mode and kernel-mode to minimize buffer
copies and kernel transitions.

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
the device. There is threfore a linear cost to encoding a message
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
