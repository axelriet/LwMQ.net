LwMQ User Guide
===============

.. note::

   Under construction.

Introduction
------------

LwMQ is born from a simple observation: there is a gap in
message-oriented inter-process communication systems.

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
or intermediate broker.

Its key differentiators include the use of transport mechanisms
that are not commonly available in other message-oriented
communication mechanisms, and often reserved to server or
datacenter environments only.

Most competing solution leverage Unix Domain Sockets (`UDS`_)
for local communications, as this transport is aligned in
the continuity of the network-based paradigms those systems
are built upon.

.. _UDS: https://en.wikipedia.org/wiki/Unix_domain_socket

In contrast, LwMQ provides its own shared-memory based
physical layer for local communication, as well as its
own data link and network layers on top if it.

The result is much improved throughput, easily in the
multi-million messages per second for small payloads,
as well as much reduced latency down to the low nanoseconds
range for single message latency while the nearest competing
solutions claim low microseconds latency.

Similarly, LwMQ leverages Remote Direct Memory Access (RDMA)
for remote peer-to-peer communication. RDMA bypasses the
network stack almost entirely (the technology is known as
"kernel bypass") and achieve higher throughput and lower
latency that what can be reached through typical network
stacks on most operating systems.

LwMQ supports three flavors of RDMA through NetworkDirect v2 providers:

   * InfiniBand: A high-bandwidth, low-latency datacenter networking technology designed for RDMA.

   * RoCE (RDMA over Converged Ethernet): Enables RDMA over Ethernet networks, allowing RDMA to function in traditional network infrastructures.

   * iWARP (Internet Wide Area RDMA Protocol): Allows RDMA over TCP/IP, which is more scalable over long distances.

LwMQ brings unprecedented local IPC performance as well
as datacenter-level network IPC performance to regular applications
running on regular workstations, provided they are equipped with
an RDMA-capable network adapter with suitable drivers. The same
of course also applies to server applications.

Workstation adapters capable of RDMA operation over iWARP or
RoCE (both atop Ethernet) can be readily obtained example from
`Broadcom`_, `NVIDIA (Mellanox)`_ or `Intel`_, while `InfiniBand`_
adapters are now common on server hardware and some high-end
workstations.

.. _Broadcom: https://techdocs.broadcom.com/us/en/storage-and-ethernet-connectivity/ethernet-nic-controllers/bcm957xxx/adapters/RDMA-over-Converged-Ethernet.html
.. _NVIDIA (Mellanox): https://docs.nvidia.com/networking/display/mlnxofedv497100lts/rdma+over+converged+ethernet+(roce)
.. _Intel: https://www.intel.com/content/www/us/en/support/articles/000031905/ethernet-products/700-series-controllers-up-to-40gbe.html
.. _InfiniBand: https://www.infinibandta.org/

Finally, LwMQ supports Hyper-V specific transports that
enable communication between the host OS, guest VMs,
and Containers without leveraging any network stack at
all. Communication can be achieved at high speed between
the host operating system and headless VMs and Containers
without any virtual network hardware.

Together, these features enable new scenarios for locally
distributed and networked application communicating without
borders at the highest throughput allowed by the hardware.

Nothing prevents LwMQ to also support network protocols
such as regular TCP or UDP but LwMQ's design revolves
around a DMA-first architecture that is optimized from
the ground up for high-speed shared-memory IPC and RDMA,
and is fundamentally a peer-to-peer communication system.

Ultrafast IPC enables many scenarios including real-time
financial data dissemination, locally-distributed agents,
AI training and reinforcement learning, delegation of
sensitive work to a separate, isolated process with
minimal impact on performance, fast batching of commands
for example to a graphic or HTML rendering engine hosted in
a separate process or container, and more.

In these scenarios, LwMQ performs *many time faster* than
existing solutions, even more so when the current solution
involves HTTP/2 over a local socket connection.

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
the designers settled for a particular the hashing function
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
and all types with "\L\M\Q\_" (and "\P\L\M\Q\_" for pointers to types) to avoid any naming collision with
other APIs or libraries.

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

Overall Diagrams
^^^^^^^^^^^^^^^^

.. mermaid::

   ---
   title: LwMQ Sender Side
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

Getting Started
---------------

Samples
-------

