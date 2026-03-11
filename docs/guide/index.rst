LwMQ User Guide
===============

.. note::

   Under construction.

Introduction
------------

LwMQ is a systems and platform architect's LEGO® set, born
from a simple observation: there is a gap in message-oriented
inter-process communication systems.

LwMQ provides messaging, caching, and key-value storage as
well as supporting features such as hashing, HMAC and
key generation, data compression, and more. Everything is
ligweighted and optimized for the best possible performance on
modern hardware.

Messaging
^^^^^^^^^

At one end of the spectrum one finds sophisticated message
brokers reachable over a network and requiring
complex deployment, configuration and maintenance, often
with companies specializing in deploying and supporting
such systems.

At the other end of the spectrum one find libraries providing
message-oriented communication often patterned after well-known
abstractions such as BSD sockets, enabling local and remote
communication between processes without an intermediate broker.

LwMQ falls on the side of the second category, providing direct
peer-to-peer inter-process communication without configuration
or intermediate party.

Its key differentiators include the use of transport mechanisms
that are not commonly available in other message-oriented
communication mechanisms, and often reserved to server or
datacenter environments only.

Most competing solution leverage Unix Domain Sockets (`UDS`_)
for local communications, as this transport is aligned in
the continuity of the network-based paradigms those systems
are built upon.

.. _UDS: https://en.wikipedia.org/wiki/Unix_domain_socket

In contrast, LwMQ provides its own shared-memory-based
physical layer for local communication, as well as its
own data link and network layers on top if it.

The result is much improved throughput, easily in the
multi-million messages per second on common hardware for small
payloads and reaching multi-GB per second with large payloads,
as well as much reduced latency down to the low nanoseconds
range for single one-way messages, while the nearest competing
solutions claim low microseconds latency.

Similarly, LwMQ leverages Remote Direct Memory Access (RDMA)
for remote peer-to-peer communication. RDMA bypasses the
network stack almost entirely (the technology is known as
"kernel bypass") and achieves higher throughput and lower
latency that what can be reached through typical network
stacks on most operating systems.

LwMQ supports three flavors of RDMA through NetworkDirect v2 providers:

   * InfiniBand: A high-bandwidth, low-latency datacenter networking technology designed for RDMA.

   * RoCE (RDMA over Converged Ethernet): Enables RDMA over Ethernet networks, allowing RDMA to function in traditional network infrastructures.

   * iWARP (Internet Wide Area RDMA Protocol): Allows RDMA over TCP/IP, which is more scalable over long distances.

LwMQ brings unprecedented local IPC performance without any
special hardware requirements as well as datacenter-level
remote IPC performance to regular applications running on regular
workstations with unprecedented ease of use, provided they are
equipped with an RDMA-capable network adapter with suitable
drivers.

The same of course also applies to server applications as well
as RDMA-capable virtual machines now available on `Google Cloud`_,
`AWS`_ and `Azure`_, provided they are equipped with suitable
NDv2 drivers.

.. _Google Cloud: https://docs.cloud.google.com/compute/docs/instances/create-vm-with-rdma
.. _AWS: https://aws.amazon.com/ec2/instance-types/accelerated-computing/
.. _Azure: https://learn.microsoft.com/en-us/azure/virtual-machines/extensions/enable-infiniband

Workstation adapters capable of RDMA operation over iWARP or
RoCE (both atop Ethernet) can be readily obtained example from
`Broadcom`_, `NVIDIA (Mellanox)`_ or `Intel`_, while `InfiniBand`_
adapters are now common on server hardware and some high-end
workstations.

.. _Broadcom: https://techdocs.broadcom.com/us/en/storage-and-ethernet-connectivity/ethernet-nic-controllers/bcm957xxx/adapters/RDMA-over-Converged-Ethernet.html
.. _NVIDIA (Mellanox): https://docs.nvidia.com/networking/display/mlnxofedv497100lts/rdma+over+converged+ethernet+(roce)
.. _Intel: https://www.intel.com/content/www/us/en/support/articles/000031905/ethernet-products/700-series-controllers-up-to-40gbe.html
.. _InfiniBand: https://www.infinibandta.org/

LwMQ does is not concerned about the details of the underlying
physical transport provided the vendor supplies the appropriate
drivers.

Finally, LwMQ supports Hyper-V specific transports that
enable communication between the Host OS, Guest VMs,
and Containers without leveraging any network stack at
all. Communication can be achieved at high speed between
the host operating system and headless VMs and Containers
having no virtual network hardware attached.

Together, these features enable new scenarios for locally
distributed and networked application communicating without
borders at the highest throughput allowed by the hardware.

Nothing prevents LwMQ to also support network protocols
such as regular TCP/IP or UDP, but LwMQ's design revolves
around a DMA-first architecture that is optimized from
the ground up for high-speed shared-memory IPC and RDMA,
and is fundamentally a peer-to-peer (1:1) communication system
where the application establishes peer connections through
LwMQ's communication channels.

In the future, LwMQ may support one-to-many (1:N) cardinality
through reliable multicast transports where the message
replication is offloaded to the network hardware, but v1
focuses on the 1:1 use case, knowing there is no intrinsic
limits on the number of channels an application can create
and use concurrently.

Ultrafast IPC enables many scenarios including real-time
financial data dissemination, locally-distributed agents,
faster AI training and reinforcement learning and inference
scenarios, delegation of sensitive work to a separate process
with minimal impact on performance, i.e. fast batching of
commands to a numeric solver, or a graphic or HTML renderer
hosted in separate process, container, VM, or computer.

In these scenarios, LwMQ performs *many time faster* than
existing solutions, even more so when the current solution
involves HTTP/2 over a local socket connection such as gRPC
or a local REST API.

Caching and Key-Value Storage
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Caching and key-value storage play supporting roles in LwMQ's
design, as they are often needed to implement applications
that leverage LwMQ's messaging capabilities.

Those features are designed to be used in conjunction with
the messaging subsystem. For example the in-memory LRU cache
can be used to store partial results in AI inference scenarios,
or to store often-served data with a precise expiration time.


Supporting Features
^^^^^^^^^^^^^^^^^^^

Supporting features such as 32, 64, and 128-bit hashing, HMACs,
and key generation, ultrafast data compression and other utility
features complete the offering and enable platform architects to
build their solutions with the best possible performance and
reliability, for example by adding checksums or HMACs to messages
to detect and prevent data corruption or tampering, or by
compressing large messages to reduce the amount of data
that needs to be transferred, all with minimal impact on performance.

Depending on the data link speed and congestions, it is often
advantageous to use a little CPU time to compress the data and
reduce the bandwith requirements for improved overall throughput.

Platform
^^^^^^^^

LwMQ is designed as a best-of-breed-performance library for
Windows, Windows Server, and Azure Windows VMs. While there is
nothing inherently platform-specific in the design, the
implementation is currently Windows-specific and leverages
Windows features and APIs to achieve the best possible
performance.

The author is a firm believer in multiplatform *solutions* but
cross-platform *code* often comes with a cost in terms of
performance and maintainability: sooner or later some compromises
are made to accommodate the lowest common denominator, and the
codebase becomes more complex and harder to maintain.

LwMQ's concepts can readily be ported to any platform and the
author looks forward to seeing native ports to other platforms,
possibly in other programming languages, in the future.

Implementation
^^^^^^^^^^^^^^

LwMQ is written in the C programming language following strict
disciplines only found in kernel developement.

The code is designed to be robust and bomb-proof, with a strong
emphasis on correctness and reliability, while providing
best-in-industry performance on every aspect. Some light touches
of C++ are used here and there, for example for the finite-state
machine that governs the state of the transports, such as created,
connecting, open, etc, is best implemented in C++.

Some small portions are written in assembly language and the code
often uses vector instructions (SIMD intrinsics) where appropriate.

LwMQ requires AVX-2 instructions (Haswell, Ryzen, or later)

Philosophy
----------

LwMQ is a low-level library. The user has control over the
type of queues that are used to post messages, between
single-producer unbounded queues, to multi-producer queues
of various types. The multi-producer queues implement
synchronization internally and therefore can be accessed
concurrently from multiple threads without special precautions
at some slight performance cost.

The single-producer queues are meant to be access strictly
from one and only one thread (as they don't perform internally
synchronization) but they offer unprecedented performances
where queuing a new message only requires a few machine
instructions without any lock.

LwMQ is therefore flexible and offers more control than
typical libraries and subsystems, and does not force unwarranted
cost to the application that don't need a particular guarantee.

Another example of the low-level philosophy that is
pervasive through the entire design is the in-memory cache,
which does not assume any particular key type.

Most caches accept keys in form of a text string, which in turn
makes assumptions about the character type and encoding, and
the designers settled for a particular hashing function
that they think will be suitable for most uses.

LwMQ makes no such assumptions but standardizes all keys as
128-bit (16-bytes) entities.

It is up to the application to supply the keys and decide how
to create them: random or non-random MAC-based GUIDs, hashed
strings or byte buffers with whatever hashing algorithm that
satisfies the application's requirements, whatever suits the
application at hand.

LwMQ provide fast functions to transform a string or an arbitrary
buffer into a key, as well as functions that create ideal
keys the application can use as record identity, but LwMQ 
does not make any assumption about the nature of the
data ending up serving as key. It directly exposes the
underlying native key type instead and give the application
full control over how to create those keys. Some applications
may find it advantageous to compute or create the keys in
advance and store them as part of the data. This avoids
the runtime cost of hashing the data each time a key is
needed.

The general philosophy is therefore to give maximum control
to the application and focus on the low-level aspects, with
a strong emphasis on bomb-proof robustness while providing
best-in-industry performances on every aspects.

Whatever your scenario, LwMQ aims to provide multiple ways
to efficiently address your needs.

The API follows a C-style design, with a flat API and
opaque types, and is designed to be easily callable from C,
C++, Rust, Go, Python and more.

The ABI (Application Binary Interface) remains stable between
releases, ensuring backward compatibility: new versions of
LwMQ will generally be drop-in replacements for previous
versions, meaning the API surface is stable and extensions
are made by adding new functions and types, rather than
modifying existing ones. Existing functions will *never* be
removed, ensuring as much as possible that older applications
can use newer versions of LwMQ without breaking.

The naming convention is to prefix all API functions with "Lmq"
and all types with "\L\M\Q\_" (and "\P\L\M\Q\_" for pointers to types)
to avoid any naming collision with other APIs or libraries.

The naming format follows the PrefixVerbNoun structure in TitleCase,
with the verb describing the action performed by the function and
the noun describing the main entity the function operates on.

For example, LmqCreateChannel creates new channel, LmqPostMessage
posts a message to a queue, and so on. No surprises.

Create verbs are complemented by Destroy verbs that free the
resources allocated by the Create functions. Open -> Close,
Allocate -> Free, and so on.

Architecture
------------

LwMQ messaging's architecture is DMA-first. What that means is
the messaging framework revolves around the high-level concept
of a memory window being "teleported" from one peer's address
space to another.

The actual transport happens either through directly shared
memory (for the IPC transport) or with the help of the network
adapter for the RDMA case, where the transfer is offloaded to
the hardware.

Other transports, such as Hyper-V's HvSocket, rely on an existing
pipe-like infrastructure, while other transports, like
a possible TCP transport, rely on pinned memory windows that
sit in-between user-mode and kernel-mode to minimize buffer
copies.

Sender Block Diagram
^^^^^^^^^^^^^^^^^^^^

The diagram below shows a particular configuration where a channel
has three input queues as an example. The queues are connected to the
input Scheduler on their consumer end.

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
before shipping the buffers to the device. It will club messages
together as they come, but it will ship the buffers as soon as
the buffers are full or the input queues are empty. The application
controls the buffer size and their count and can therefore tune
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
for example when using a task or concurrency framework, LwMQ
supplied multi-producer queues that implement internal locking.

A channel can support an arbitrary number of transport and even
multiple instances of the same transport, all of course subjected
to system limits. When multiple transports are attached to a
channel, the messages are laid down to each transport buffer
separately. This is necessary as LwMQ often does not own the
transport buffer: it often belongs to the device.

Messages must fit in the transport buffers, so the application
must size the buffers accordingly. If a message is larger than
the transport buffer size, it is rejected by the message encoder
and an error is returned to the application. This is the only
way to ensure true atomic message delivery as any particular
message is either send and delivered in its entirety or not
at all.

LwMQ's channels don't not maintain state related to messages: the
application creates messages independently of any channel or connection
and adds data frames and content to the message as it sees fit.

The messages are only associated with a channel when the application
posts it to one of the channel's input queues. This solves many
issues that inherently prevent other messaging systems from
supporting true multithreaded operations and true atomic message
transport.

Not maintaining state at the channel level also solves
issues inherent to request-response patterns where an application
can post any number of concurrent requests without waiting for
any answer, while competing solutions often require the application
to wait for the response to a request before posting the next one,
which is a major bottleneck in concurrent scenarios and also causes
the sender to be stuck indefinitely if the receiver fails to respond
for any reason.

LwMQ solves all those problems and more, while providing
best-in-industry performance and a simple API that can be learned
and put to work in a half day.

.. mermaid::

   ---
   title: LwMQ Sender Side
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

Receiver Block Diagram
^^^^^^^^^^^^^^^^^^^^^^

On the receiving side, transports are serviced by threads that
collects messages from incoming transport buffers. The decoded
messages are then posted to the channel's output queue.

.. mermaid::

   ---
   title: LwMQ Receiver Side
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

Getting Started
---------------

Samples
-------

