LwMQ User Guide
===============

.. note::

   Under construction.

Introduction
------------

LwMQ is born from a simple observation: there is a gap in
the message-oriented inter-process communication systems.

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
peer-to-peer inter-process communication without configuration.

Its key differentiators include the use of transport mechanisms
that are not commonly available in other message-oriented
communication mechanisms.

Most competing solution leverage Unix Domain Sockets (UDS)
for local communication as this transport is aligned in
the continuity of the network-based paradigms those systems
are built upon.

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
latency that what can be achieved through typical network
stacks.

LwMQ supports three flavors of RDMA through NetworkDirect v2 providers:

   * InfiniBand: A high-bandwidth, low-latency networking technology designed for RDMA.

   * RoCE (RDMA over Converged Ethernet): Enables RDMA over Ethernet networks, allowing RDMA to function in traditional network infrastructures.

   * iWARP (Internet Wide Area RDMA Protocol): Allows RDMA over TCP/IP, which is more scalable over long distances.

Finally, LwMQ supports Hyper-V specific transports that
enable communication between the host OS and guest VMs
and Containers without leveraging any network stack at
all. Communication can be achieved at high speed between
the host operating system and headless VMs and Containers
without any virtual network hardware.

Together, these features enable new scenarios for locally
distributed and networked application communicating without
borders at the highest throughput allowed by the hardware.

Nothing prevents LwMQ to also support network protocols
such as regular TCP or UDP but LwMQ's design revolves
around a DMA-first architecture that is optimized for
high-speed shared-memory IPC and RDMA.

Ultrafast IPC enable many scenarios including real-time
financial data dissemination, locally-distributed agents,
AI training and reinforcement learning, delegation of
sensitive work to a separate, isolated process with
minimal impact on performance, fast batching of commands
for example to a graphic or HTML rendering engine hosted in
a separate process, ane more.

Philosophy
----------

Architecture
------------

Getting Started
---------------

Samples
-------

