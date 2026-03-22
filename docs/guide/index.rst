###############
LwMQ User Guide
###############

Introduction
------------

LwMQ is a systems and platform architect's LEGO® set, born
from a simple observation: there is a gap in message-oriented
inter-process communication systems.

LwMQ provides messaging, caching, and key-value storage as
well as supporting features such as hashing, HMAC and
key generation, data compression, and more. Everything is
lightweight and optimized for the best possible performance on
modern hardware.

Messaging
^^^^^^^^^

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

LRU Caching and Key-Value Storage
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

LRU caching and key-value storage play supporting roles in LwMQ's
offerings, as they are often needed to implement applications
that leverage LwMQ's messaging capabilities.

Those features are designed to be used in conjunction with
the messaging subsystem. For example the in-memory LRU cache
can be used to store partial results in AI inference scenarios,
or to store often-served data with a precise expiration time.

The segmented LRU cache supports massive caching scenarios with
millions of entries under *heavy* multithreaded concurrent access,
and both caches support compression and encryption with per-entry
entropy.

Both the LRU cache and the segmented LRU cache support multi-million
retrievals per second, only subject to CPU speed and memory bandwidth
limitations, with the segmented cache taking the lead in heavily
contended scenarios, delivering lookup and retrieval rates in the tens
of millions per second on suitable hardware.

The Key-Value storage enable persistency with optional compression
at reasonably high speeds, the database being backed by virtual memory,
and offers a decent tradeoff between speed and durability.

Supporting Features
^^^^^^^^^^^^^^^^^^^

Supporting features such as ultrafast 32/64/128-bit hashing, HMACs,
and key generation, hyperfast LZ4 data compression and other utility
features complete the offering and enables platform architects to
build their solutions with the best possible performance and
reliability, for example by adding checksums or HMACs to messages
to detect and prevent data corruption or tampering, or by
compressing large messages to reduce the amount of data
that needs to be transferred, all with the least possible impact on
performance.

Depending on the data link speed and congestions, it is sometimes
advantageous to use some CPU time to compress/decompress data and
reduce the bandwidth requirements for improved overall throughput.

Platform
^^^^^^^^

LwMQ is designed as a best-of-breed-performance library for
Windows, Windows Server, and Windows VMs hosted on Azure or elsewhere.

While there is nothing inherently platform-specific in the design, the
implementation is currently Windows-specific and leverages
Windows features and APIs to achieve the best possible
performance.

The author is a firm believer in multi-platform *solutions* but
cross-platform *code* often comes with a cost in terms of
performance and maintainability: sooner or later compromises
are made to accommodate the lowest common denominator, and the
codebase becomes more complex and harder to maintain.

LwMQ's concepts can readily be ported and the author looks
forward to seeing native ports to other platforms,
possibly in other programming languages, in the future.

Implementation
^^^^^^^^^^^^^^

LwMQ is written in the C programming language following strict
disciplines mostly found in kernel development and mission-critical,
long-running software. It is suitable for *always on* appliance-type
software with indefinite uptimes.

The C code is compiled as C++ to benefit from stricter type checking
of the latter language, a technique now used in the Windows kernel
and other critical software projects, and the project takes maximum
advantage of static analysis for compile-time defect detection.

The code is designed to be robust and bomb-proof, with a strong
emphasis on correctness and reliability, while providing
best-in-industry performance on every aspect.

Some light touches of proper C++ are used here and there, for example
the finite-state machine that governs the state of the transports
is best implemented in C++ [`GoF`_, p. 305ff]

.. _GoF: https://a.co/d/02cPzhzs

Small portions are written in assembly language and the code
often uses vector instructions (SIMD intrinsics) where appropriate.

LwMQ *requires* AVX-2 instructions (Haswell, Ryzen, or later) and
performs exceptionally well on contemporary hardware.

Some features, including compression and encryption and some hashing
functions, make use of specialized CPU instructions and can take advantage
of recent advances, such as AES-NI and AVX-512, on suitable hardware
(e.g. Skylake or later) thanks to runtime dispatching. LwMQ squeezes
every bit of performance from your hardware!

Philosophy
----------

LwMQ is a low-level library. The user has control over the
type of queues that are used to post messages, between
single-producer unbounded queues, to multi-producer queues
of various types. The multi-producer queues implement
synchronization internally and therefore can be accessed
concurrently from multiple threads without special precautions
at some performance cost.

The single-producer queues are meant to be access strictly
from one and only one thread (as they don't perform internal
synchronization) but they offer unprecedented performances
where queuing a new message only requires a few machine
instructions without taking any lock.

LwMQ is therefore flexible and offers more control than
typical libraries and subsystems, and does not force unwarranted
cost to applications that don't need a particular guarantee.

Another example of the low-level philosophy that is
pervasive through the entire design is the in-memory LRU cache,
which does not assume any particular key type.

Most existing LRU caches accept keys in form of a text string, which
in turn makes assumptions about the character type and encoding,
and the cache designers settled for a particular hash function
that they think should be suitable for most uses.

LwMQ makes no such assumptions and standardizes all keys as
128-bit (16-bytes) entities.

It is up to the application to supply the keys and decide how
to create them: random or non-random MAC-based GUIDs, hashed
ANSI, UTF-8, or Unicode strings or byte buffers hashed with whatever
hashing algorithm satisfying the application's requirements.

LwMQ provide fast functions to transform any string or arbitrary
byte buffer into a key, as well as functions that create ideal
keys the application can also use as record identity, but LwMQ 
does not make any assumption about the nature of the
data ending up serving as key. It directly exposes the
underlying native key type instead and give the application
full control over how to create those keys. Some applications
may find it advantageous to compute or create the keys in
advance and store them as part of their data. This avoids
the runtime cost of hashing the data each time a key is
needed. The drawback is it is the application's responsibility
to ensure the uniqueness of the keys and make them fit into 16 bytes.

The general philosophy is to give maximum control to the application
and focus on the low-level aspects, with a strong emphasis on
bomb-proof robustness while providing best-in-industry
performances on every aspects.

Whatever your scenario, LwMQ aims to provide multiple ways
to efficiently address your needs.

As an example, instead of opting for a turnkey "distributed cache" solution,
consider that LwMQ provides the individual building blocks that can be used
to assemble your own, tailored to your specific needs and requirements,
and optimized for your particular scenario which, with a low-contention highly
concurrent LRU backend and a fast RDMA front-end will likely
outperform any existing solution by a wide margin, augmented with the
inherent elasticity provided by queuing.

Need a *secure* cache? Flip a switch to enable fast memory encryption for tiny
entries and AES-GCM encryption for large ones. Need a *compressed* cache?
Flip another switch to enable LZ4 compression. Need a *compressed and
encrypted* cache? Flip both switches.

The cache also optionally support hardware deflate through `Intel QAT`_ for
very large payloads, if your server is equipped with the required accelerator
and drivers.

.. _Intel QAT: https://www.intel.com/content/www/us/en/architecture-and-technology/intel-quick-assist-technology-overview.html

Ease of use, best in class components that can be composed to
build your winning solution. That, in a nutshell, is the philosophy
behind LwMQ.

API
---

The API follows a C-style design, with a flat API and
opaque types, and is designed to be easily callable from C,
C++, Rust, Go, Python and more. Wrappers and bindings can
also be created for almost any language.

The ABI (Application Binary Interface) remains stable between
releases, ensuring backward compatibility: new versions of
LwMQ will generally be drop-in replacements for any previous
version, meaning the API surface is stable and extensions
are made by adding new functions and types, rather than
modifying existing ones. Existing functions will *never* be
removed, ensuring as much as possible that older applications
can use newer versions of LwMQ unmodified and without breaking.

The naming convention is to prefix all API functions with "Lmq"
and all types with "\L\M\Q\_" (and "\P\L\M\Q\_" for pointers to types)
to avoid any naming collision with other APIs or libraries.

The naming format follows the PrefixVerbNoun structure in TitleCase,
with the verb describing the action performed by the function and
the noun describing the main entity the function operates on.

For example, LmqCreateChannel() creates a new communication channel,
LmqPostMessage() posts a message to a queue, and so on. No surprises.

Create verbs are complemented by Destroy verbs that free the
resources allocated by the Create functions. Open -> Close,
Allocate -> Free, and so on.

Save for a couple of rare exceptions, LwMQ's API calls return a status
code (HRESULT) indicating success or failure.

BugChecks (Fail Fast)
^^^^^^^^^^^^^^^^^^^^^

LwMQ embraces a *fail fast* philosophy: when a critical error is detected,
the process will BugCheck (crash) immediately to prevent any potential
data corruption or security breach. This occurs when a clear programmer
error is detected, for example when passing an invalid object handle
to an API function. Another reason to fail fast is to capture the state
of the process as close as possible from the detection of the problem,
often making diagnosis far easier simply by having the right call stack.

This topic is always controversial in programming circles and subject
to heated debates, but the author firmly believes that in a low-level library
such as LwMQ, which is designed to be used in performance-critical and
mission-critical scenarios, it is better to fail fast (and loud!) rather
than silently continue in a potentially corrupted state.

LwMQ components does not fail fast for common errors, but are very strict
about correct usage. By making misuses immediately apparent, LwMQ helps
developers detect and fix bugs early in the development cycle, rather
than allowing them to manifest later in production where they can
cause more harm.

Validation
""""""""""

Examples of regular error return during parameter validation:

.. code:: cpp

   RETURN_HR_IF_MSG(E_INVALIDARG,
                    BufferSizeBytes < 1,
                    "BufferSizeBytes must be at least 1.");

   RETURN_HR_IF_MSG(E_INVALIDARG,
                    MaximumSizeBytes == 0,
                    "MaximumSizeBytes must of > 0, realistically "
                    "some number of megabytes/gigabytes.");

There are approximately 300 locations across the codebase where LwMQ
performs parameter validation and checks for error conditions and
returns errors to the caller.

Fail Fast
"""""""""

Example of a bugcheck for a critical programmer error:

.. code:: cpp

   if (LmqiIsValidWorkspace(WorkspaceInternal) == FALSE)
   {
      FAIL_FAST_HR_MSG(E_INVALIDARG,
                       "Invalid compression workspace handle.");
   }

Example of a bugcheck for a critical internal error (i.e. a condition
that should never occur regardless of circumstances, and is therefore
likely a bug that must be addressed):

.. code:: cpp

   if (LastEntry == nullptr)
         [[unlikely]]
   {
         FAIL_FAST_HR_MSG(E_UNEXPECTED,
                          "LastEntry should not be null.");
   }

There are approximately 150 locations across the codebase where LwMQ
performs critical validations and would bugcheck on fatal errors.

Tracing and Logging
"""""""""""""""""""

Note that LwMQ uses the error tracing/logging and reporting
macros provided by the `Window Internal (or Implementation) Libraries (WIL)`_ to
provide detailed error messages and context to the developer when a failure occurs,
which can be invaluable for debugging and troubleshooting. Most errors have
an associated message that provides succinct details about the specific error
condition, as illustrated above.

LwMQ also makes *modest* use of the Windows TraceLogging facilities to log
significant events which can be enabled and collected with tools such as
`Windows Performance Recorder (WPR) and Windows Performance Analyzer (WPA)`_
for in-depth analysis of the system's behavior, or simply viewed in
real-time with tools such as `TraceView`_.

.. _Window Internal (or Implementation) Libraries (WIL): https://github.com/microsoft/wil
.. _TraceView: https://learn.microsoft.com/en-us/windows-hardware/drivers/devtest/traceview
.. _Windows Performance Recorder (WPR) and Windows Performance Analyzer (WPA): https://learn.microsoft.com/en-us/windows-hardware/test/wpt/windows-performance-analyzer

The ETW provider GUID for the messaging component is:

.. code:: cpp

   //
   // "api-lwmq-messaging"
   //
   // {7da8eabe-76ce-556e-7a7e-63e75c6c78f5}
   //

Debug Symbols
^^^^^^^^^^^^^

We publish debug symbols for all LwMQ binaries to our
symbol server. Debugging tools can be configured to
automatically retrieve our public symbols when debugging
or profiling applications that use LwMQ.

The symbol server URL is:

.. code:: cpp

   https://www.lwmq.net/symbols

There is no user-browseable content at that address but
debugging tools know how to use it.

Visual Studio can be configured to use our symbol server by
adding it to the list of symbol servers in the debugging
options: Tools->Options->Debugging->Symbols.

Exceptions
^^^^^^^^^^

LwMQ does not use exceptions.

Telemetry
^^^^^^^^^^

LwMQ does not emit telemetry.

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
pipe-like infrastructure, while yet other transports, like
a possible TCP transport, rely on pinned memory windows and queues
that sit in-between user-mode and kernel-mode to minimize buffer
copies and kernel transitions.

Sender Block Diagram
^^^^^^^^^^^^^^^^^^^^

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
^^^^^^^^^^^^^^^^^^^^^^

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

Licensing
---------

LwMQ's licensing terms are not decided at this time.

Inquiries should be directed to info@lwmq.net

Getting Started
---------------

LwMQ is deployed in seconds using Windows Installer packages, and can be
used in any application by referencing the appropriate header files and
linking against the provided libraries.

Installation
^^^^^^^^^^^^

LwMQ ships in two distinct packages: the "Runtime" package,
which contains the DLLs and other files needed to run applications
that use LwMQ, and the "SDK" package which contains the header files,
libraries, and other files needed to develop applications that use LwMQ.

LwMQ is Datacenter and Host OS-ready: all LwMQ binaries and setup packages
are `digitally signed`_ with our DigiCert Extended Validation (EV) code
signing certificate to ensure their authenticity and integrity with the
highest level of trust.

.. _digitally signed: https://learn.microsoft.com/en-us/windows-hardware/drivers/install/authenticode

Deployment Packages
"""""""""""""""""""

LwMQ is deployed through standard Windows Installer (MSI) packages, which
can be easily installed and managed on any Windows system.

.. table::
   :width: 95%
   :widths: 33, 66
   :align: left

   ==============  ==============================================
   Package Name    Description
   ==============  ==============================================
   lwmq.setup.msi  Runtime components for applications using LwMQ
   ==============  ==============================================

This MSI installation database deploys the runtime components of LwMQ to
the target computer. Every application *should* deploy the runtime package
to ensure the correct version of the runtime components. The LwMQ installer
takes care of counting references and to make sure the latest version of
the runtime components is present on the system.

Similarly, applications *should* uninstall the runtime package when they
are uninstalled: the Windows Installer subsystem takes care of removing
the components when they are no longer needed after the last application
that installed them gets uninstalled.

Alternatively, system administrators can also install the runtime package
in their system image or application layer and manage it through their
standard fleet update and maintenance process, for example through
`Microsoft Configuration Manager`_ or other endpoint management system.

.. _Microsoft Configuration Manager: https://learn.microsoft.com/en-us/intune/configmgr/

The installer and all binary files it contains are digitally signed
using our DigiCert Extended Validation (EV) code signing certificate.

Size: The total size of the installed files is approximately 4MB.

Registry entries, environment variables, and configuration files: None.

.. table::
   :width: 95%
   :widths: 33, 66
   :align: left

   ==================  =============================================
   Package Name        Description
   ==================  =============================================
   lwmq.sdk.setup.msi  Development files for applications using LwMQ
   ==================  =============================================

This MSI installation database deploys the development files (headers,
libraries, debug symbols, samples) needed to develop applications that use LwMQ.

The SDK *also* deploys the runtime components, so installing the SDK
is a one-stop solution for development and CI/CD build machines. However,
the SDK is not meant to be deployed on production systems.

Finally, the SDK includes the redistributable **lwmq.setup.msi** package
so the application being built, and in particular its setup system, can
reference the runtime from a well-known location.

The default installation path for the SDK is **"C:\\Program Files\\LwMQ.SDK"** and contains the following subfolders:

.. table::
   :width: 95%
   :widths: 33, 66
   :align: left

   =============  ========================================================================
   Subfolder      Description
   =============  ========================================================================
   inc            Header files needed to develop applications
   -------------  ------------------------------------------------------------------------
   lib            Import libraries needed to link applications
   -------------  ------------------------------------------------------------------------
   pdb            Debug symbols
   -------------  ------------------------------------------------------------------------
   tools          Command-line tools for testing LwMQ
   -------------  ------------------------------------------------------------------------
   redist         Redistributable files
   =============  ========================================================================

Samples & Tools
---------------

TBD (Sample programs will be available on the `GitHub page`_)

.. _GitHub page: https://github.com/axelriet/LwMQ.net

.. important::
   
   The test tools supploed with the SDK, ``client_thr.exe`` and ``server_thr.exe`` require
   the ``SeCreateGlobalPrivilege`` when using the IPC protocol, as described on
   the :doc:`../transports/ipc` page.

   The IPC protocol is the ONLY protocol available in the March 22, 2026 ALPHA (and DEMO)
   release of the SDK.