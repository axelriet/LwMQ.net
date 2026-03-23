#########
Messaging
#########

Messaging is a core feature of LwMQ. It enables processes
to exchange *messages*, which are structured entities composed
of one or more *data frame* and which are sent atomically.

At one end of the spectrum of existing messaging systems one
finds sophisticated message brokers reachable over a network
and requiring complex deployment, configuration and maintenance,
often with companies specializing in deploying and supporting
such systems. These systems often offer strong guarantees such
as *exactly once* delivery, persistence, message replay, and more.

At the other end of the spectrum one find libraries providing
message-oriented communication often patterned after well-known
abstractions such as BSD sockets, enabling local and remote
communication between processes without an intermediate broker.

These system are lightweight in the sense that they require
less (or no) administration, but also offer less guarantees,
often in exchange for more speed.

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
payloads, and reaching multi-GB per second throughput with large
payloads, as well as much reduced latency down to the low
nanoseconds range for single one-way messages, while the
nearest competing solutions claim low microseconds latency.

Similarly, LwMQ leverages Remote Direct Memory Access (RDMA)
for remote peer-to-peer communication. RDMA is a datacenter
technology that bypasses the network stack almost entirely (the
technology is known as "kernel bypass") and achieves higher
throughput and lower latency that what can be reached through
typical network stacks on most operating systems.

LwMQ supports three flavors of RDMA through NetworkDirect v2 providers:

   * InfiniBand: A high-bandwidth, low-latency datacenter networking technology designed for RDMA.

   * RoCE (RDMA over Converged Ethernet): Enables RDMA over Ethernet networks, allowing RDMA to function in traditional network infrastructures.

   * iWARP (Internet Wide Area RDMA Protocol): Allows RDMA over TCP/IP, which is more scalable over long distances.

LwMQ brings unprecedented local IPC performance without special
hardware or software requirements, as well as datacenter-level
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
drivers. Furthermore, LwMQ users are entirely shielded from the
underlying transport details and can leverage any of the supported
transports with virtually no change to the code.

Finally, LwMQ supports Hyper-V specific transports that
enable communication between the Host OS, Guest VMs,
and Containers without leveraging any network stack at
all. Communication is achieved at high speed between
the host operating system and potentially headless VMs
and Containers having no virtual network hardware attached.

Together, these features enable new scenarios for locally
distributed and networked application communicating without
borders at the highest throughput allowed by the hardware.

Nothing prevents LwMQ to also support classic network protocols
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
faster AI training, reinforcement learning and inference
scenarios, delegation of sensitive work to a separate process
with minimal impact on performance, i.e. fast batching of
commands to a numeric solver, or a graphic or HTML renderer
hosted in separate process, container, VM, or computer.

In these scenarios, LwMQ performs *many time faster* than
existing solutions, even more so when the current solution
involves HTTP/2 over a local socket connection such as gRPC
or a local REST API, not mentioning elastic queues, queue
priorities, and other features unique to LwMQ.
