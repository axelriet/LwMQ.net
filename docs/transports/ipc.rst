******************
LwMQ IPC Transport
******************

LwMQ's IPC transport is a high-performance inter-process
communication transport based on shared memory. It is designed
for low latency and high throughput communication between
processes on the same machine, and can achieve multi-millions
of messages per second with nanosecond-level latency on
modern hardware.

Uri Format
==========

The Uri format for the IPC transport is: ``ipc://address[:port]``

Where:

   - ``address`` is the name of the shared memory section to use for communication. It can be any string, but it is recommended to use a unique name to avoid conflicts with other applications. The recommended format is company/product/channel_name. Keep the length *less than 120 characters*.
   - ``port`` is an optional parameter that can be used to specify a port number for the transport. This parameter is optional, is not a real port number and can be anything that fits into an unsigned short, except zero: the range is therefore [1..65535] 

The format is deliberately chosen to look familiar to users of network transports,
while being flexible enough to allow for future extensions and variations
of the transport. The ``port`` parameter is not used by the IPC transport
itself, but can be used by applications to differentiate between different
channels or instances of the transport.

LwMQ's IPC transport does not use the file system for communication, and therefore does
not require a file path, and no file gets created on the file system for the transport.

There are no particular convention or special characters for the address, except that
it is passed as a null-terminated Unicode (UTF-16) string, and that the total length
of the address part should be *less than 120 characters* to fit into LwMQ's internal
address representation. The only reserved character is the colon (":") which is
used to separate the address from the optional port number, and therefore cannot
be used in the address itself. The scheme ("ipc://") and the optional port number
do *not* count against the less than 120 characters limit for the address.

.. important::

   The IPC transport requires the processes using it to have the necessary
   permissions to create and access shared memory sections on the system.
   On Windows, this requires the processes to have the ``SeCreateGlobalPrivilege``.

   By default, only Services and members of the Administrator group
   can create global objects but an administrator can grant this privilege
   to any user account or group, for example Authenticated Users,
   to allow any process or service running under that account or group to use
   the IPC transport.
   
   You can assign the required privilege to the account or the group the account
   belongs to using the Local Security Policy editor (type 'local security policy'
   in the start menu)
   
   Local Security Policy
      Security Settings
         Local Policies
            User Rights Assignment
               Create global objects
                  Properties
                     Add User or Group...

Adding an IPC Transport
=======================

Instance of the IPC transports are added to channels using the ``LmqAddTransport()`` function, with the transport descriptor being an Uri in the above format.

.. code:: cpp

   LMQAPI
   LmqAddTransport (
      LMQ_CHANNEL Channel,
      PCWSTR TransportDescriptor,
      SIZE_T BufferSizeBytes,
      LONG MaxPendingSendBuffers,
      LONG MaxPendingReceiveBuffers,
      UINT32 CreationFlags,
      PLMQ_TRANSPORT Transport
      );

Where:

   - ``TransportDescriptor`` - For example "ipc://mycompany/myproduct/mychannel:1234"
   - ``BufferSizeBytes`` - Some large figure able to accommodate at lease one copy of the largest message expected to be sent over the transport, for example 16*1024 for 16KB messages + 1KB for encoding overhead. To allow message clubbing, consider a larger size, for example 1MB + 4KB. In server scenarios you can consider much larger sizes depending on expected usage.
   - ``MaxPendingSendBuffers`` - The maximum number of send buffers that can be pending at any given time. This should be set to a value that is large enough to accommodate the expected message traffic, but not too large to cause excessive memory usage. Consider *at least* two send buffers, and expect peak performance with the IPC transport around 4 to 8 buffers.
   - ``MaxPendingReceiveBuffers`` - The maximum number of receive buffers that can be pending at any given time. This should be set to a value that is large enough to accommodate the expected message traffic, but not too large to cause excessive memory usage. Consider *at least* two receive buffers, and expect peak performance with the IPC transport around 4 to 8 buffers.
   - ``CreationFlags`` - TRANSPORT_CREATIONFLAGS_SEND, TRANSPORT_CREATIONFLAGS_RECEIVE, or TRANSPORT_CREATIONFLAGS_SENDRECEIVE. If a transport only *sends*, the number of *receive* buffers must be zero, and vice-versa.
   - ``Transport`` - [Optional] Receives the created transport instance for use with ``LmqTransportControl()``. In most instances, simply pass a null.

.. important:: If adding multiple instances of *any* transport to the *same* channel, the ``BufferSizeBytes`` must be identical across all instances of all transports on that channel. The number of pending send/receive buffers can vary according to the transport type and traffic characteristics.

Control Codes
=============

The IPC transport supports the following control codes for use with ``LmqTransportControl()``:

   - ``LMQ_TRANSPORTCONTROL_SETCLIENTCONNECTIONRETRYINTERVALMS`` - (ULONG) Sets the retry interval in milliseconds for client connections. The default value is 100ms.

.. code:: cpp

    LMQAPI
    LmqTransportControl (
        LMQ_TRANSPORT Transport,
        ULONG ControlCode,
        PVOID InputBuffer,
        ULONG InputBufferLengthBytes,
        PVOID OutputBuffer,
        ULONG OutputBufferLengthBytes,
        PULONG BytesReturned
        );

Where:

    - ``Transport`` - The transport instance to control, obtained from ``LmqAddTransport()``.
    - ``ControlCode`` - One of the control codes shown above.
    - ``InputBuffer`` - A pointer to a buffer that contains the input data for the control code. For example, a pointer to an ULONG value that specifies the retry interval in milliseconds for client connections.
    - ``InputBufferLengthBytes`` - The length of the input buffer in bytes. For example, sizeof(ULONG) for an ULONG value.
    - ``OutputBuffer`` - A pointer to a buffer that receives the output data for the control code. This parameter is optional and must be null if the control code does not produce any output data.
    - ``OutputBufferLengthBytes`` - The length of the output buffer in bytes. This parameter is optional and must be zero if the control code does not produce any output data.
    - ``BytesReturned`` - A pointer to a ULONG variable that receives the number of bytes returned in the output buffer. This parameter is optional and must be null if the control code does not produce any output data.

Remarks
=======

The IPC transport has an arbitrary "server" side that creates the shared memory section,
and an arbitrary "client" side that opens an existing shared section. Arbitrary
because there really is no "server" or "client" but one side still need to
create the section (similar to the side who binds/listen in a network connection)
while the other side "connects".

The "client" side retries and connects in the background using a timer. As
such, it does not matter which side starts first. In fact, any side can
start and begin sending messages (subject to queue policies and buffer sizes) while the
other side is not yet running.

This solves the important problem of start-order dependencies found in
nearly all existing solutions: the server must start and begin listening
before the client can connect.

Relaxing start-order dependencies is crucial in complex scenarios where
many agents / processes interact with each other.

The deferred connection is a core feature of LwMQ and applies to all
transports. However, the IPC transport in LwMQ does not rely on any
network stack, and has no direct visibility on the process lifetimes
of the connected peers.

Application Crashes
^^^^^^^^^^^^^^^^^^^

In classic scenarios, the network stack really is a separate tier and
has a lifetime on its own as a kernel component. The kernel can easily
detect and notify interested parties of process terminations, including
in case of crashes, making it possible for a network stack to
close connections and clean up after a connected process has exited for
any reason, including surprise-exits due to application crashes.

The IPC transport in LwMQ *currently* does not have this luxury. Shipping
a kernel component on Windows requires a lot of administrative burden
as drivers must be EV-signed *and* co-signed by Microsoft, or the kernel
won't load them in recent versions of the OS, and disabling secure boot
and relaxing the driver-signing policies simply is not acceptable for
the production scenarios that LwMQ aims to fullfil.

The ultimate goal is the LwMQ IPC transport is crash-resilient on both
ends of the connection. The loss of in-flight message must be understood
to be an unfortunate side effect of a process crashing. It goes the same
for network connection where any data buffered or packet queued but not
yet sent will be lost. However, we are working hard to ensure reasonable
behavior in case of application crash, where the side that has crashed
can reconnect seamlessly if restarted.

