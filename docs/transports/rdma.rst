*******************
LwMQ RDMA Transport
*******************

The RDMA transport (upcoming) enables direct
peer-to-peer bi-directional communication between
pairs of processes in different machines, physical
or virtual.

The transport leverages special capabilities
of the network adapters and requires appropriate
drivers and NetworkDirect v2 providers, available
from the network hardware manufacturers.

LwMQ fully abstracts the network provider details
and enables seamless RDMA connections simply
by using "rdma:" as connection scheme.

For example, `rdma://localhost:5555` will find
the first RDMA-capable adapter on the machine and bind
to it on port 5555. It's that easy.

All of LwMQ's capabilities are available on the RDMA
transport, including raw buffer access and the
entire set of message queuing capabilities.

The actual data movements are offloaded to
the network adapter and buffer transfer happens
truly asynchronously.

Network adapters have limitations regarding the
maximum memory aperture size they support and
those limitations of course apply to the LwMQ
RDMA transport too. You can query the buffer limits
of the transport through the LwMQ API to learn
what those limits are on your particular network
adapter and network provider combination.

When communicating across a network the entire
path from source to destination must be RDMA-capable
and RDMA-enables. This often requires complex
configuration of both the network driver, RDMA
provider, operating system, and any hardware (routers etc)
standing in between machines.

We recommend to test the full network path using
the tools supplied by the network card manufacturer
before attempting to use RDMA from LwMQ.
