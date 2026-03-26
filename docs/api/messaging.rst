.. |us| unicode:: U+00B5 s

******************
LwMQ Messaging API
******************

This section documents LwMQ's core messaging APIs, which are transport-agnostic and form the basis for LwMQ's messaging capabilities.

These APIs allow applications to create messages, append data frames to messages, and post ad receive messages through communication channels.

The messaging APIs are designed for high performance with high throughput and low latency through a DMA-first underlying architecture, making them suitable for a wide range of applications that require efficient ultra-high-performance message-based communication.

C and C++ Header File
=====================

.. code:: cpp

    #include <api-lwmq-messaging.h>

Binaries
========

.. code:: cpp

    api-lwmq-messaging-1-0.dll

Dependencies
============

.. code:: cpp

    api-lwmq-heap-1-0.dll
    api-lwmq-time-1-0.dll
    api-lwmq-hash-1-0.dll

Overview
========

Types
-----

.. code:: cpp

    PLMQ_CHANNEL
    LMQ_CHANNELTYPE
    LMQ_CHANNELROLE
    LMQ_CHANNELCONTROLCODE
    LMQ_RECEIVEQUEUETYPE
    LMQ_SENDQUEUE
    LMQ_SENDQUEUETYPE
    LMQ_SENDQUEUEPRIORITY
    LMQ_TRANSPORT
    LMQ_TRANSPORTBUFFER
    LMQ_TRANSPORTBUFFERLIMITS
    LMQ_MESSAGE
    LMQ_MESSAGECALLBACK
    LMQ_MESSAGEFRAMEENUMHINT

Initialization and Termination
------------------------------

.. code:: cpp

    #define LMQ_CURRENT_API_VERSION MAKEWORD(1, 0)

    LMQAPI
    LmqInitialize (
        WORD VersionRequested
        );

    LMQAPI
    LmqTerminate (
        VOID
        );

Channels
--------

.. code:: cpp

    LMQAPI
    LmqCreateChannel (
        LMQ_CHANNELTYPE ChannelType,
        PLMQ_CHANNEL Channel
        );

    LMQAPI
    LmqOpenChannel (
        LMQ_CHANNEL Channel,
        LMQ_CHANNELROLE ChannelRole,
        LMQ_RECEIVEQUEUETYPE ReceiveQueueType,
        LONG ReceiveQueueCapacity
        );

    LMQAPI
    LmqChannelControl (
        LMQ_CHANNEL Channel,
        LMQ_CHANNELCONTROLCODE ControlCode,
        PVOID InputBuffer,
        ULONG InputBufferLengthBytes,
        PVOID OutputBuffer,
        ULONG OutputBufferLengthBytes,
        PULONG BytesReturned
        );

    LMQAPI
    LmqFlushChannel (
        LMQ_CHANNEL Channel,
        UINT32 TimeoutMs
        );

    LMQAPI
    LmqCloseChannel (
        LMQ_CHANNEL Channel,
        UINT32 LingerTimeoutMs
        );

    LMQAPI
    LmqDestroyChannel (
        PLMQ_CHANNEL Channel
        );

Queues
------

.. code:: cpp

    LMQAPI
    LmqAddSendQueue (
        LMQ_CHANNEL Channel,
        LMQ_SENDQUEUETYPE QueueType,
        LMQ_SENDQUEUEPRIORITY QueuePriority,
        LONG Capacity,
        PLMQ_SENDQUEUE SendQueue
        );

Transports
----------

.. code:: cpp

    LMQAPI
    LmqQueryTransportBufferLimits (
        PCWSTR TransportDescriptor,
        PLMQ_TRANSPORTBUFFERLIMITS BufferLimits,
        ULONG BufferLimitsSizeBytes
        );

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

Direct Buffer Access
--------------------

.. code:: cpp

    LMQAPI
    LmqRawChannelGetTransportBuffer (
        LMQ_TRANSPORT Transport,
        UINT32 TimeoutMs,
        PLMQ_TRANSPORTBUFFER* Buffer
        );

    LMQAPI
    LmqRawChannelSendTransportBuffer (
        LMQ_TRANSPORT Transport,
        PLMQ_TRANSPORTBUFFER* Buffer,
        BOOL EraseBufferAfterSending
        );

    LMQAPI
    LmqRawChannelReceiveTransportBuffer (
        LMQ_TRANSPORT Transport,
        UINT32 TimeoutMs,
        PLMQ_TRANSPORTBUFFER* Buffer
        );

    LMQAPI
    LmqRawChannelDisposeTransportBuffer (
        LMQ_TRANSPORT Transport,
        PLMQ_TRANSPORTBUFFER* Buffer,
        BOOL EraseBufferBeforeDisposing
        );

Messages & Frames
-----------------

.. code:: cpp

    LMQAPI
    LmqCreateMessage (
        USHORT FrameCountHint,
        PLMQ_MESSAGE Message
        );

    LMQAPI
    LmqCreateMessageRef (
        LMQ_MESSAGE Message,
        PLMQ_MESSAGE Clone
        );

    LMQAPI
    LmqAppendFrame (
        LMQ_MESSAGE Message,
        const BYTE* Data,
        UINT64 DataSize,
        ULONG64 Timestamp
        );

    LMQAPI
    LmqAppendStaticFrame (
        LMQ_MESSAGE Message,
        const BYTE* Data,
        UINT64 DataSize,
        ULONG64 Timestamp
        );

    LMQAPI
    LmqAppendExternalFrame (
        LMQ_MESSAGE Message,
        const BYTE* Data,
        UINT64 DataSize,
        ULONG64 Timestamp,
        PLMQ_MESSAGECALLBACK Callback,
        PVOID Context
        );

    LMQAPI
    LmqGetFrameCount (
        LMQ_MESSAGE Message,
        PUSHORT FrameCount,
        PUINT64 PayloadSizeBytes
        );

    LMQAPI
    LmqGetFrameData (
        LMQ_MESSAGE Message,
        USHORT FrameIndex,
        const BYTE** Data,
        PUINT64 DataSize,
        PULONG64 Timestamp,
        PLMQ_MESSAGEFRAMEENUMHINT EnumHint
        );

    LMQAPI
    LmqInitFrameEnumHint (
        _Pre_invalid_ PLMQ_MESSAGEFRAMEENUMHINT EnumHint
        );

    LMQAPI
    LmqQueryMaxInlineFrameSize (
        PUSHORT MaxInlineFrameSize
        );

    LMQAPI
    LmqDestroyUnpostedMessage (
        PLMQ_MESSAGE Message
        );

Post and Receive Messages
-------------------------

.. code:: cpp

    LMQAPI
    LmqPostMessage (
        LMQ_SENDQUEUE SendQueue,
        PLMQ_MESSAGE Message,
        UINT32 TimeoutMs
        );

    LMQAPI
    LmqPostMessageWithTag (
        LMQ_SENDQUEUE SendQueue,
        PLMQ_MESSAGE Message,
        LONG_PTR Tag,
        UINT32 TimeoutMs
        );

    LMQAPI
    LmqReceiveMessage (
        LMQ_CHANNEL Channel,
        UINT32 TimeoutMs,
        PUSHORT FrameCount,
        PUINT64 PayloadSizeBytes,
        PLMQ_MESSAGE Message
        );

    LMQAPI
    LmqDisposeReceivedMessage (
        PLMQ_MESSAGE Message
        );

Initialization and Termination
==============================

Macros
------

.. c:macro:: LMQ_CURRENT_API_VERSION

Functions
---------

.. c:function:: LMQAPI LmqInitialize(WORD VersionRequested)

    Initializes LwMQ and allocates internal global ressources.

    :param VersionRequested: Pass the supplied LMQ_CURRENT_API_VERSION macro.

    .. note:: This function must be called before any other LwMQ function. Multiple calls are allowed but must be matched by a corresponding call to LmqTerminate().

.. c:function:: LMQAPI LmqTerminate()

    Shuts down LwMQ and deallocate internal ressources.
    
    .. note:: This function must be called once for each calls made to LmqInitialize().

.. code:: cpp

    #include <api-lwmq-messaging.h>

    HRESULT hr = LmqInitialize(LMQ_CURRENT_API_VERSION);

    // ...

    hr = LmqTerminate();

Messages
========

Messages are structured entities composed of one or more data frames. The
general philosophy is the messages belong to LwMQ and the application only holds
references to messages, which are automatically freed by LwMQ once they are no
longer needed. This design allows for efficient message handling and minimizes
the risk of memory leaks or dangling pointers in the application.

.. code:: cpp

    //
    // General idea with most error checking
    // and details omitted for clarity.
    //

    LMQ_MESSAGE Message;

    hr = LmqCreateMessage(LMQ_MESSAGEFRAMECOUNT_DEFAULT,
                          &Message);

    hr = LmqAppendFrame(Message,
                        MessageContent,
                        MessageSize,
                        Timestamp);

    //
    // Queue the message for sending. After this call, the
    // application can't access the message anymore as the
    // ownership has been transferred to LwMQ, and it will
    // be asynchronously destroyed at some future point.
    //

    if (FAILED(LmqPostMessage(SendQueue,
                              &Message,
                              TimeoutMs)))
    {
        //
        // Messages that failed to post can be retried or
        // must be destroyed to free their resources.
        //

        LmqDestroyUnpostedMessage(&Message);
    }

Types
-----

.. c:type:: LMQ_MESSAGE;

    An opaque message instance.

Functions
---------

.. c:function:: LMQAPI LmqCreateMessage(USHORT FrameCountHint, PLMQ_MESSAGE Message)

    Creates a new message instance.

    :param FrameCountHint: A hint for the number of frames that will be added to the message. For most applications, pass LMQ_MESSAGEFRAMECOUNT_DEFAULT.

    :param Message: A pointer to a variable that receives the created message instance.

    .. note:: The FrameCountHint value is indicative and does not represent a limit. LwMQ may optimize internal allocations knowing the number of frames that will be added to a message, but applications are not required to provide an accurate hint. Extremely performance-sensitive applications benefit from using single-frame messages, which is the default value used when passing LMQ_MESSAGEFRAMECOUNT_DEFAULT.
        
.. code:: cpp
    
    LMQ_MESSAGE Message;

    HRESULT hr = LmqCreateMessage(LMQ_MESSAGEFRAMECOUNT_DEFAULT,
                                  &Message);

.. c:function:: LMQAPI LmqCreateMessageRef(LMQ_MESSAGE Message, PLMQ_MESSAGE MessageRef)

    :param Message: The message to create a reference from.

    :param MessageRef: A pointer to a variable that receives the created message reference. The reference shares the same data frames as the original message and is independent of the original message's lifetime.

    .. note:: Creating a message reference offers a way to send the same message through more than one communication channel. The normal behavior is the application returns ownership of the message to LwMQ when queuing them for sending. Since sending messages is asynchronous, the application cannot keep a reference to the message sent, as it will be destroyed asynchronously at some point in the future. Creating a message reference allows the application to send the original and retain a reference to be sent through another channel. Unsent references must be destroyed through LmqDestroyUnpostedMessage(). LwMQ takes care of reference counting and manages the lifetime of the messages handed back to it. Creating a message reference is a lightweight operation, as the reference shares the same data frames as the original message. The reference is independent of the original message's lifetime, and can be safely used even after the original message has been sent and destroyed by LwMQ.

.. code:: cpp
    
    LMQ_MESSAGE MessageRef;

    HRESULT hr = LmqCreateMessageRef(Message,
                                     &MessageRef);

.. c:function:: LMQAPI LmqAppendFrame(LMQ_MESSAGE Message, const BYTE* Data, UINT64 DataSize, ULONG64 Timestamp)

    :param Message: The message to append the frame to.

    :param Data: A pointer to the frame data. The data is copied into the message and can be freed or reused by the caller after the function returns.

    :param DataSize: The size of the frame data in bytes. Note that small payloads are copied "inline" without requiring additional internal memory allocation, therefore improving performance. Use LmqQueryMaxInlineFrameSize() to determine the maximum payload size that can be stored inline, if that is important for your application.

    :param Timestamp: An optional timestamp associated with the frame, or one of three special values: LMQ_TIMESTAMP_NONE, LMQ_TIMESTAMP_USE_SYSTEMTIME, suitable for most applications, or LMQ_TIMESTAMP_USE_SYSTEMTIME_PRECISE for applications requiring the the highest possible level of precision (\<1 |us|) at some performance expense.

    .. note:: If supplied explicitly, the timestamp must be specified in Windows NT time format: a 64-bit integer representing the number of 100-nanosecond intervals that have elapsed since January 1, 1601. All timestamps are in UTC format so they are not affected by changes in time zone or daylight saving time.

.. code:: cpp
    
    HRESULT hr = LmqAppendFrame(Message,
                                (BYTE*)"Hello, World!", // Copied.
                                13,
                                LMQ_TIMESTAMP_NONE);

.. c:function:: LMQAPI LmqAppendStaticFrame(LMQ_MESSAGE Message, const BYTE* Data, UINT64 DataSize, ULONG64 Timestamp)

    :param Message: The message to append the frame to.

    :param Data: A pointer to the frame data. The data is not copied into the message and must remain valid for the lifetime of the communication channel to which the message will be queued, as well as the lifetime of any message references created from the message. Static frames are intended to send static data, for example a static string, or some static binary data outliving the lifespan of the communication channel. The data will be accessed asynchronously by LwMQ when sending messages, and the application must ensure that the data remains valid until the message is sent and all message references are destroyed, which is guaranteed to be the case if the data is static or global. Static frames are not copied into the message, therefore they can be more efficient for sending static data that is reused across multiple messages, for example a common header or a common payload.

    :param DataSize: The size of the frame data in bytes. The data is not copied into the message.

    :param Timestamp: An optional timestamp associated with the frame. See the Timestamp parameter of LmqAppendFrame() for important details.

.. code:: cpp
    
    HRESULT hr = LmqAppendStaticFrame(Message,
                                      (BYTE*)"Hello, World!",
                                      13,
                                      LMQ_TIMESTAMP_USE_SYSTEMTIME);

.. c:function:: LMQAPI LmqAppendExternalFrame(LMQ_MESSAGE Message, const BYTE* Data, UINT64 DataSize, ULONG64 Timestamp, PLMQ_MESSAGECALLBACK Callback, PVOID Context)

    :param Message: The message to append the frame to.

    :param Data: A pointer to the frame data. The data is not copied into the message and must remain valid until the callback is invoked with the RELEASE_DATA reason.

    :param DataSize: The size of the frame data in bytes. The data is not copied into the message.

    :param Timestamp: An optional timestamp associated with the frame. See the Timestamp parameter of LmqAppendFrame() for important details.

    :param Callback: A callback function that will be invoked when the message is no longer needed by the communication channel. The callback is responsible for freeing or otherwise releasing the frame data. The callback is also called with a reason of ACQUIRE_DATA when the message is being prepared for sending, allowing the application to perform any necessary preparations on the data before it is sent, for example pinning memory or flushing CPU caches. The callback will be called at exactly once with a reason of ACQUIRE_DATA, and exactly once with a reason of RELEASE_DATA.

    :param Context: An optional user-defined context pointer or value that will be passed to the callback function.

.. code:: cpp

    //
    // From <api-lwmq-messaging.h>
    //

    typedef enum _LQM_MESSAGECALLBACKREASON
    {
        ACQUIRE_DATA,
        RELEASE_DATA
    }
    LQM_MESSAGECALLBACKREASON;

    typedef HRESULT(REENTRANT_CALLBACK* PLMQ_MESSAGECALLBACK) (
                    LQM_MESSAGECALLBACKREASON Reason,
                    const BYTE* Data,
                    UINT64 DataSize,
                    PVOID Context);

    //
    // Example of appending an external frame with a callback to
    // release the data when the message is no longer needed by
    // the communication channel.
    //        
    
    HRESULT hr = LmqAppendExternalFrame(Message,
                                        DataBuffer,
                                        DataBufferSizeBytes,
                                        LMQ_TIMESTAMP_NONE,
                                        CallbackFunction,
                                        CallbackContext);

.. c:function:: LMQAPI LmqGetFrameCount(LMQ_MESSAGE Message, PUSHORT FrameCount, PUINT64 PayloadSizeBytes)

    :param Message: The message to query.

    :param FrameCount: A pointer to a variable that receives the number of frames in the message.

    :param PayloadSizeBytes: An optional pointer to a variable that receives the total size of all frame payloads in bytes.

.. code:: cpp
    
    USHORT FrameCount;
    UINT64 PayloadSizeBytes;

    HRESULT hr = LmqGetFrameCount(Message,
                                  &FrameCount,
                                  &PayloadSizeBytes);

.. c:function:: LMQAPI LmqGetFrameData(LMQ_MESSAGE Message, USHORT FrameIndex, const BYTE** Data, PUINT64 DataSize, PULONG64 Timestamp, PLMQ_MESSAGEFRAMEENUMHINT EnumHint)

    :param Message: The message to query.

    :param FrameIndex: The zero-based index of the frame to query.

    :param Data: A pointer to a variable that receives a pointer to the frame data wthin the message. The data is read-only and owned by the message. It must not be modified or freed by the caller.

    :param DataSize: A pointer to a variable that receives the size of the frame data in bytes.

    :param Timestamp: An optional pointer to a variable that receives the timestamp associated with the frame. The value is zero if no timestamp was associated with the frame. See the Timestamp parameter of LmqAppendFrame() for important details.

    :param EnumHint: An optional pointer to an enumeration hint structure that can be used to optimize enumeration of multiple frames in a message. If supplied, the caller must initialize the hint via LmqInitFrameEnumHint(). The enumeration hint is valid for all subsequent calls to LmqGetFrameData() for the same message until either the message is destroyed or LmqInitFrameEnumHint() is called again for the same enumeration hint structure.

.. code:: cpp

        const BYTE* Data;
        UINT64 DataSizeBytes;
        ULONG64 Timestamp;
    
        HRESULT hr = LmqGetFrameData(Message,
                                     FrameIndex,
                                     &Data,
                                     &DataSizeBytes,
                                     &Timestamp,
                                     nullptr); // Hint not used.
    
.. c:function:: LMQAPI LmqInitFrameEnumHint(PLMQ_MESSAGEFRAMEENUMHINT EnumHint)

    :param EnumHint: A pointer to an opaque enumeration hint structure to initialize.

.. code:: cpp

    const BYTE* Data;
    UINT64 DataSizeBytes;
    ULONG64 Timestamp;
    LMQ_MESSAGEFRAMEENUMHINT EnumHint;

    HRESULT hr = LmqInitFrameEnumHint(&EnumHint);

    // ...

    hr = LmqGetFrameData(Message,
                         FrameIndex,
                         &Data,
                         &DataSizeBytes,
                         &Timestamp,
                         &EnumHint);

.. note:: The use of an enumeration hint is optional but recommended for performance reasons when enumerating multiple frames in a message with many frames. Moreover, the performance is optimal when accessing frames in monotonic increasing order.

.. c:function:: LMQAPI LmqQueryMaxInlineFrameSize(PUSHORT MaxInlineFrameSize)

    :param MaxInlineFrameSize: A pointer to a variable that receives the maximum frame payload size in bytes that can be stored "inline" within the message without requiring additional internal memory allocation.

    .. note:: The value is indicative and does not represent a limit. The value is static and depends on LwMQ's build, therefore you only need to call this function once per application lifetime. Applications that are extremely performance-sensitive benefit from keeping their data frame's payloads at or below this size, as well as indicating the expected number of frames they intend to add to a message upon message creation.

.. code:: cpp

    USHORT MaxInlineFrameSizeBytes;
    
    HRESULT hr = LmqQueryMaxInlineFrameSize(&MaxInlineFrameSizeBytes);

.. c:function:: LMQAPI LmqDestroyUnpostedMessage(PLMQ_MESSAGE Message)

    Destroys a message that was created but not sent. This function is only necessary if the message (or message reference) was created but will not be sent, for example because the application encountered an error after creating the message but before sending it.

    :param Message: A pointer to the message to destroy. The pointer is set to NULL before the function returns.

    .. note:: Messages that are successfully posted (queued for sending) do not need to be explicitly destroyed, as they are automatically destroyed by LwMQ once they are no longer needed. However, if a message or message reference was created but will not be sent, for example because the application encountered an error after creating the message or reference but before sending it, then this function must be called to free the resources associated with the message, and the message or reference itself.

.. code:: cpp

    LMQ_MESSAGE Message;

    HRESULT hr = LmqCreateMessage(LMQ_MESSAGEFRAMECOUNT_DEFAULT,
                                  &Message);

    if (SUCCEEDED(hr)
    {
        //
        // The message has been successfuly created but
        // an error occured afterward such as you could
        // not acquire the data to send, or you changed
        // your mind about sending the message.
        //
        // In this case you must destroy the message to
        // free its memory resources, since the message
        // will not be handed out to LwMQ and thus will
        // not be automatically destroyed.
        //

        if (SOME_ERROR_CONDITION)
        {
            //
            // The message that will never be sent.
            //

            hr = LmqDestroyUnpostedMessage(&Message);
        }
    }

Channels
========

Types
-----

.. c:type:: PLMQ_CHANNEL

    An opaque channel instance handle.

.. cpp:type:: LMQ_CHANNELTYPE

    An enumerated type representing the type of a communication channel.    

.. code:: cpp

    typedef enum _LMQ_CHANNELTYPE
    {
        LMQ_CHANNELTYPE_ONE_TO_ONE = 1
    }
    LMQ_CHANNELTYPE;

.. cpp:type:: LMQ_CHANNELROLE

    An enumerated type representing the role of a communication channel once opened.

.. code:: cpp

    typedef enum _LMQ_CHANNELROLE
    {
        LMQ_CHANNELROLE_CLIENT = 1,
        LMQ_CHANNELROLE_SERVER
    }
    LMQ_CHANNELROLE;

.. cpp:type:: LMQ_RECEIVEQUEUETYPE

    An enumerated type representing the type of receive queue to use for incoming messages when adding one or more transports as receivers.

.. code:: cpp
    
    typedef enum _LMQ_RECEIVEQUEUETYPE
    {
        LMQ_RECEIVEQUEUETYPE_NA,
        LMQ_RECEIVEQUEUETYPE_MONOCONSUMER_UNBOUNDED,
        LMQ_RECEIVEQUEUETYPE_MULTICONSUMER_UNBOUNDED,
        LMQ_RECEIVEQUEUETYPE_MULTICONSUMER_BOUNDED_DISCARD_OLDEST,
        LMQ_RECEIVEQUEUETYPE_MULTICONSUMER_BOUNDED_DISCARD_NEWEST
    }
    LMQ_RECEIVEQUEUETYPE;

.. cpp:type:: LMQ_CHANNELCONTROLCODE

    An enumerated type representing the control codes for performing a control operation on a communication channel.

.. code:: cpp
    
    typedef enum _LMQ_CHANNELCONTROLCODE
    {
        LMQ_CHANNELCONTROLCODE_SERVICETHREADIDEALPROCESSOR,
        LMQ_CHANNELCONTROLCODE_SERVICETHREADGROUPAFFINITY
    }
    LMQ_CHANNELCONTROLCODE;

Functions
---------

.. c:function:: LMQAPI LmqCreateChannel(LMQ_CHANNELTYPE ChannelType, PLMQ_CHANNEL Channel)

    Creates a new communication channel.

    :param ChannelType: The type of the communication channel to create.
    
    :param Channel: A pointer to a variable that receives the created channel instance.

.. code:: cpp

    LMQ_CHANNEL Channel;

    HRESULT hr = LmqCreateChannel(LMQ_CHANNELTYPE_ONE_TO_ONE,
                                  &Channel);

.. c:function:: LMQAPI LmqOpenChannel(LMQ_CHANNEL Channel, LMQ_CHANNELROLE ChannelRole, LMQ_RECEIVEQUEUETYPE ReceiveQueueType, LONG ReceiveQueueCapacity)

    Opens a communication channel for sending or receiving messages.

    :param Channel: The communication channel to open.

    :param ChannelRole: The role that the channel will play once opened, either server or client. While there is no proper server or client in LwMQ, you can think of the server as the side that binds and listens, and the client as the side who connects.

    :param ReceiveQueueType: If the channel has any transport that can receive data, the type of receive queue to use for incoming messages. If the channel has multiple transports that can receive data, they will all share the same receive queue. This parameter must be LMQ_RECEIVEQUEUETYPE_NA if the channel has no transport that can receive data.

    :param ReceiveQueueCapacity: If the channel has any transport that can receive data and the receive queue type is one of the bounded types, the maximum number of messages that can be queued for receiving at any given time. This parameter must be zero if the channel does not have a bounded receive queue or if the channel has no transport that can receive data.

    .. note:: Transports must be added before the channel is opened, and cannot be added once the channel is opened. The channel will not be able to be opened until at least one transport has been added. All parameters must be consistent. For example you must indicate a queue type if any transport can receive, you can only indicate a capacity only if the queue is bounded, etc.

.. code:: cpp

    //
    // Create channel, add transport, add send
    // queue if needed, then open the channel.
    //

    HRESULT hr = LmqOpenChannel(Channel,
                                LMQ_CHANNELROLE_SERVER,
                                LMQ_RECEIVEQUEUETYPE_MONOCONSUMER_UNBOUNDED,
                                LMQ_QUEUECAPACITY_UNBOUNDED);

.. c:function:: LMQAPI LmqChannelControl(LMQ_CHANNEL Channel, LMQ_CHANNELCONTROLCODE ControlCode, PVOID InputBuffer, ULONG InputBufferLengthBytes, PVOID OutputBuffer, ULONG OutputBufferLengthBytes, PULONG BytesReturned)
    
    Performs a control operation on a communication channel.

    :param Channel: The communication channel to perform the control operation on.

    :param ControlCode: The control code representing the control operation to perform.

    :param InputBuffer: An optional pointer to a buffer containing input data for the control operation.

    :param InputBufferLengthBytes: The size of the input buffer in bytes.

    :param OutputBuffer: An optional pointer to a buffer that receives output data from the control operation.

    :param OutputBufferLengthBytes: The size of the output buffer in bytes.

    :param BytesReturned: An optional pointer to a variable that receives the size of the data returned in the output buffer in bytes.

    The following channel control codes are supported:

        * LMQ_CHANNELCONTROLCODE_SERVICETHREADIDEALPROCESSOR
            
            Set the channel service thread's ideal processor.

            Input: Pointer to an initialized `PROCESSOR_NUMBER <https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-processor_number>`_ structure.
            
            Output: [Optional] Pointer a PROCESSOR_NUMBER structure receiving the previous processor number.  

        * LMQ_CHANNELCONTROLCODE_SERVICETHREADGROUPAFFINITY
            
            Set the channel service thread's group affinity.
            
            Input: pointer to an initialized `GROUP_AFFINITY <https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-group_affinity>`_ structure.
            
            Output: [Optional] Pointer a GROUP_AFFINITY structure receiving the previous group information.  

.. code:: cpp

    HRESULT hr = LmqChannelControl(Channel,
                                   ControlCode,
                                   InputBuffer,
                                   InputBufferLengthBytes,
                                   OutputBuffer,
                                   OutputBufferLengthBytes,
                                   &BytesReturned);

.. c:function:: LMQAPI LmqFlushChannel(LMQ_CHANNEL Channel, UINT32 TimeoutMs)

    Flushes a communication channel, ensuring that all pending messages are sent.

    :param Channel: The communication channel to flush.

    :param TimeoutMs: The maximum time to wait for the flush operation to complete, in milliseconds. A value of INFINITE can be used to wait indefinitely.

    .. note:: This function blocks until all pending messages are sent, or until the specified timeout elapses. If the flush operation does not complete within the specified timeout, the function returns a timeout error code, but any pending messages or send buffers will still be processed, assuming the communication link is still active. You can call this function periodically when sending messages at a sustained rapid pace through an unbounded queue, if message transport or processing is not fast enough on the other side of the channel, which causes the number of queued messages on the sending side to keep growing. Flushing the channel allows the application to apply backpressure and avoid excessive memory usage. The timeout is not necessarily millisecond-accurate as the thread can enter a sleeping state while waiting for the channel to be flushed.

.. code:: cpp

    //
    // Wait up to 1 second for the channel to be flushed.
    //

    HRESULT hr = LmqFlushChannel(Channel,
                                 1000);

.. c:function:: LMQAPI LmqCloseChannel(LMQ_CHANNEL Channel, UINT32 LingerTimeoutMs)
    
    Closes a communication channel.

    :param Channel: The communication channel to close.

    :param LingerTimeoutMs: The maximum time to wait for any pending messages to be sent before forcefully closing the channel, in milliseconds. A value of INFINITE can be used to wait indefinitely.

    .. note:: If LingerTimeoutMs > 0 the function blocks until all pending messages are sent and all received messages are processed by the applicationbefore closing the channel. If the flush operation does not complete within the LingerTimeoutMs time, the channel is forcefully closed anyway. The timeout is not necessarily millisecond-accurate as the thread can enter a sleeping state while waiting for the channel to be flushed.

.. code:: cpp

    //
    // Wait up to 1 second for any queued messages
    // to be sent before closing the channel.
    //

    HRESULT hr = LmqCloseChannel(Channel,
                                 1000);

.. c:function:: LMQAPI LmqDestroyChannel(PLMQ_CHANNEL Channel)
    
    Destroys a communication channel.

    :param Channel: A pointer to the communication channel to destroy.

.. code:: cpp

    HRESULT hr = LmqDestroyChannel(&Channel);

Send Queues
===========

Types
-----

.. c:type:: LMQ_SENDQUEUE

    An opaque send queue instance handle.

.. c:type:: LMQ_SENDQUEUETYPE

    An enumerated type representing the type of a send queue.

.. code:: cpp

    typedef enum _LMQ_SENDQUEUETYPE
    {
        LMQ_SENDQUEUETYPE_NA,
        LMQ_SENDQUEUETYPE_MONOPRODUCER_UNBOUNDED,
        LMQ_SENDQUEUETYPE_MULTIPRODUCER_UNBOUNDED,
        LMQ_SENDQUEUETYPE_MULTIPRODUCER_UNBOUNDED_TAGGED,
        LMQ_SENDQUEUETYPE_MULTIPRODUCER_BOUNDED_DISCARD_OLDEST,
        LMQ_SENDQUEUETYPE_MULTIPRODUCER_BOUNDED_DISCARD_NEWEST,
        LMQ_SENDQUEUETYPE_MULTIPRODUCER_BOUNDED_TAGGED_DISCARD_OLDEST,
        LMQ_SENDQUEUETYPE_MULTIPRODUCER_BOUNDED_TAGGED_DISCARD_NEWEST
    }
    LMQ_SENDQUEUETYPE;

.. c:type:: LMQ_SENDQUEUEPRIORITY

    An enumerated type representing the priority of a send queue relative to other send queues on the same channel.
   
.. code:: cpp

    typedef enum _LMQ_SENDQUEUEPRIORITY
    {
        LMQ_SENDQUEUEPRIORITY_TIME_CRITICAL,
        LMQ_SENDQUEUEPRIORITY_HIGHEST,
        LMQ_SENDQUEUEPRIORITY_ABOVE_NORMAL,
        LMQ_SENDQUEUEPRIORITY_NORMAL,
        LMQ_SENDQUEUEPRIORITY_BELOW_NORMAL,
        LMQ_SENDQUEUEPRIORITY_LOWEST,
        LMQ_SENDQUEUEPRIORITY_IDLE
    }
    LMQ_SENDQUEUEPRIORITY;

Functions
---------

.. c:function:: LMQAPI LmqAddSendQueue(LMQ_CHANNEL Channel, LMQ_SENDQUEUETYPE QueueType, LMQ_SENDQUEUEPRIORITY QueuePriority, LONG Capacity, PLMQ_SENDQUEUE SendQueue)
    
    Adds a send queue to a communication channel.

    :param Channel: The communication channel to add the send queue to.

    :param QueueType: The type of the send queue to add.

    :param QueuePriority: The priority of the send queue relative to other send queues on the same channel. This parameter is only relevant if the channel has multiple send queues. For most use cases, pass LMQ_SENDQUEUEPRIORITY_NORMAL.

    :param Capacity: The maximum number of messages that can be queued for sending at any given time in the send queue, provided the queue type is one of the bounded types. This parameter must be zero if the queue type is unbounded.

    :param SendQueue: A pointer to a variable that receives the created send queue instance.

    .. note:: A channel can have multiple send queues, but only one receive queue. Transports are added to the channel, and messages are sent through a specific send queue on the channel. Each message is sent though each sending transport in the channel. The send queue priority is used to determine the order in which messages are sent when there are multiple send queues on the same channel. Messages queued on higher priority send queues tend to be sent before messages queued on lower priority send queues, but LwMQ uses a weigthed round-robin scheduling algorithm to ensure some fairness among send queues. Two special priorities are not subjected to the prioritized scheduling: LMQ_SENDQUEUEPRIORITY_TIME_CRITICAL and LMQ_SENDQUEUEPRIORITY_IDLE. All pending messages queued on a LMQ_SENDQUEUEPRIORITY_TIME_CRITICAL send queue are always sent before messages on all other priority levels, possibly starving other queues, and messages queued on a LMQ_SENDQUEUEPRIORITY_IDLE send queue are always sent after all other messages, possibly getting starved. The two extreme priorities are useful for applications that need to send some messages with very low latency, for example to trigger some action on the receiving side, or to send some messages very low importance, for example some telemetry data that is nice to have on the receiving side but not worth delaying the delivery of other messages. Messages at the same priority are sent in order. Messages can capture the timestamp at which they were posted (queued for sending), which is useful for example for progress messages where the recipient can compute accurate rates based on the queuing time and not the reception time, which is subject to queuing latencies induced by traffic congestions or interruptions, and fluctuations in the actual transport time.

.. code:: cpp

    //
    // Example of adding an unbounded monoproducer send queue to a channel.
    //

    LMQ_SENDQUEUE SendQueue;

    HRESULT hr = LmqAddSendQueue(Channel,
                                 LMQ_SENDQUEUETYPE_MONOPRODUCER_UNBOUNDED,
                                 LMQ_SENDQUEUEPRIORITY_NORMAL,
                                 LMQ_QUEUECAPACITY_UNBOUNDED,
                                 &SendQueue);

Transports
==========

Types
-----

.. c:type:: LMQ_TRANSPORT

    An opaque transport instance handle.

.. c:struct:: LMQ_TRANSPORTBUFFERLIMITS

    A structure representing the buffer limits of a transport.

.. code:: cpp

    typedef struct _LMQ_TRANSPORTBUFFERLIMITS
    {
        SIZE_T MaxBufferSizeBytes;
        LONG MaxPendingSendBuffers;
        LONG MaxPendingReceiveBuffers;
        ULONG MaxInlineDataSize;
    }
    LMQ_TRANSPORTBUFFERLIMITS, *PLMQ_TRANSPORTBUFFERLIMITS;

.. c:struct:: LMQ_TRANSPORTBUFFER

    A structure representing a raw transport buffer.

.. code:: cpp

    typedef struct _LMQ_TRANSPORTBUFFER
    {
        //
        // When returned from LmqRawChannelGetTransportBuffer(), contains
        // the length of the buffer = maximum bytes to write
        // and must be set to effective bytes to send before
        // calling LmqRawChannelSendTransportBuffer().
        //
        // When returned from LmqRawChannelReceiveTransportBuffer(), this
        // contains the available data length, and therefore
        // the maximum number of bytes to read from the buffer.
        //

        SIZE_T BufferSizeBytes;

        const PBYTE Buffer;
    }
    LMQ_TRANSPORTBUFFER, *PLMQ_TRANSPORTBUFFER;

Functions
---------

.. c:function:: LMQAPI LmqAddTransport(LMQ_CHANNEL Channel, PCWSTR TransportDescriptor, SIZE_T BufferSizeBytes, LONG MaxPendingSendBuffers, LONG MaxPendingReceiveBuffers, UINT32 CreationFlags, PLMQ_TRANSPORT Transport)

    Adds a transport to a communication channel.

    :param Channel: The communication channel to add the transport to.

    :param TransportDescriptor: The descriptor of the transport to add. The format of the transport descriptor is transport-specific. For example, for a RDMA transport, the descriptor is in the format "rdma://hostname:port" while the IPC transport uses the format "ipc://some-arbitrary-string".

    :param BufferSizeBytes: The size of the buffers to use for sending and receiving messages through the transport, in bytes. See the note below for important details about the buffer size.

    :param MaxPendingSendBuffers: The maximum number of send buffers that can be pending for sending through the transport at any given time. This parameter must be zero if the transport cannot send, or at least 1 otherwise. For best sending performance, multiple send buffers are recommended to allow for fully asynchronous overlapped operations. The sweet spot is transport-dependent but generally tends to be around 4-16 buffers.

    :param MaxPendingReceiveBuffers: The maximum number of receive buffers that can be pending for receiving through the transport at any given time. This parameter must be zero if the transport cannot receive, or at least 1 otherwise. For best receiving performance, multiple receive buffers are recommended to allow for fully asynchronous overlapped operations. The sweet spot is transport-dependent but generally tends to be around 4-16 buffers.

    :param CreationFlags: Flags controlling the creation of the transport, either TRANSPORT_CREATIONFLAGS_SEND, TRANSPORT_CREATIONFLAGS_RECEIVE, or TRANSPORT_CREATIONFLAGS_SENDRECEIVE.

    :param Transport: An optional pointer to a variable that receives the created transport instance.

    .. note:: The bufer size parameter must be at least as large as the largest single message, plus the frame and message header wire encoding overhead, that will be sent or received through the transport. Ideally, the buffer size should be much larger than the typical message size, which enables message clubbing when messages are sent in rapid successions. For example, an application that sends, say, 80 bytes messages at very high rate (millions per second) will greatly benefit from using large buffers in the 1MB range. Moreover, all transports added to a particular channel must share the same buffer size. The actual maximum message size that cen be sent for a particular buffer size depends on various factors as the wire format use variable-length encoding for compacity. As a rule of thumb, consider ~1% overhead. For large payload, consider single-frame messages and add 4096 bytes to the expected payload size: if sending data in single-frame messages with 1MB of payload data (1,048,576 bytes) each - assuming the underlying transport supports that size - use a buffer size of 1,048,576 + 4,096 = 1,052,672 which allows for up to 4KB bytes of overhead, which is more than enough for the frame and message headers while aligning the buffer to a page boundary. Make sure you use at least 3-4 such buffers so your app can generate the next payload while the previous ones are transmitted. If sending messages with multiple frames or messages with very small payloads, the overhead can be higher in percentage, therefore you might want to use a larger buffer size to achieve optimal performance. There is no exact guidance regarding the optimal buffer size and count for each application. You need to experiment with different values to find the optimal configuration for your application, transport, and workload. The transport instance can be used to perform transport-specific control operations on the transport through LmqTransportControl(). This parameter is optional.

.. code:: cpp
    
    HRESULT hr = LmqAddTransport(Channel,
                                 L"ipc://mycompany/myapp/myport",
                                 1052672,  // 1MB payload + 4KB overhead.
                                 8,        // Send buffers.
                                 8,        // Receive buffers.
                                 TRANSPORT_CREATIONFLAGS_SENDRECEIVE,
                                 nullptr); // Instance not needed.

.. c:function:: LMQAPI LmqQueryTransportBufferLimits(PCWSTR TransportDescriptor, PLMQ_TRANSPORTBUFFERLIMITS BufferLimits, ULONG BufferLimitsSizeBytes)

    Queries the buffer limits for a transport.

    :param TransportDescriptor: The descriptor of the transport to query.

    :param BufferLimits: A pointer to a variable that receives the buffer limits of the transport.

    :param BufferLimitsSizeBytes: The size of the BufferLimits structure in bytes. This parameter allows for future extensibility of the BufferLimits structure. It must be set to sizeof(LMQ_TRANSPORTBUFFERLIMITS).

.. code:: cpp
    
    LMQ_TRANSPORTBUFFERLIMITS BufferLimits;

    HRESULT hr = LmqQueryTransportBufferLimits(TransportDescriptor,
                                               &BufferLimits,
                                               sizeof(BufferLimits));

.. c:function:: LMQAPI LmqTransportControl(LMQ_TRANSPORT Transport, ULONG ControlCode, PVOID InputBuffer, ULONG InputBufferLengthBytes, PVOID OutputBuffer, ULONG OutputBufferLengthBytes, PULONG BytesReturned)

    Performs a control operation on a transport.

    :param Transport: The transport to perform the control operation on.

    :param ControlCode: The control code representing the control operation to perform. This parameter is transport-specific.

    :param InputBuffer: An optional pointer to a buffer containing input data for the control operation.

    :param InputBufferLengthBytes: The size of the input buffer in bytes.

    :param OutputBuffer: An optional pointer to a buffer that receives output data from the control operation.

    :param OutputBufferLengthBytes: The size of the output buffer in bytes.

    :param BytesReturned: An optional pointer to a variable that receives the size of the data returned in the output buffer in bytes.

.. code:: cpp

    HRESULT hr = LmqTransportControl(Transport,
                                     ControlCode,
                                     InputBuffer,
                                     InputBufferLengthBytes,
                                     OutputBuffer,
                                     OutputBufferLengthBytes,
                                     &BytesReturned);    

Direct Buffer Access
^^^^^^^^^^^^^^^^^^^^

.. c:function:: LMQAPI LmqRawChannelGetTransportBuffer(LMQ_TRANSPORT Transport, UINT32 TimeoutMs, PLMQ_TRANSPORTBUFFER* Buffer)

    Obtains a pointer to a transport buffer for sending a raw data block.

    :param Transport: The transport to obtain the buffer from.

    :param TimeoutMs: The maximum time to wait for a buffer to be available, in milliseconds. A value of INFINITE can be used to wait indefinitely. If the function fails with a timeout error code, it means no buffer became available within the specified timeout, but any buffer that becomes available later can still be obtained by calling this function again.

    :param Buffer: A pointer to a variable that receives a pointer to the obtained transport buffer.

.. c:function:: LMQAPI LmqRawChannelSendTransportBuffer(LMQ_TRANSPORT Transport, PLMQ_TRANSPORTBUFFER* Buffer, BOOL EraseBufferAfterSending)

    Sends a raw data block through a transport using a transport buffer obtained from LmqRawChannelGetTransportBuffer().

    :param Transport: The transport to send the buffer through.
    
    :param Buffer: A pointer to the transport buffer to send.
    
    :param EraseBufferAfterSending: If TRUE, the buffer content is securely erased by overwriting it with zeros before the buffer is reused for sending another message or receiving data. This is useful when the buffer contains sensitive data that should not remain in memory after being sent. Note that enabling this option can have a performance impact, especially for large buffers, as it requires an additional memory operation to overwrite the buffer content.

.. c:function:: LMQAPI LmqRawChannelReceiveTransportBuffer(LMQ_TRANSPORT Transport, UINT32 TimeoutMs, PLMQ_TRANSPORTBUFFER* Buffer)

    Receives a raw data block through a transport using a transport buffer.

    :param Transport: The transport to receive the buffer from.

    :param TimeoutMs: The maximum time to wait for a buffer with received data to be available, in milliseconds. A value of INFINITE can be used to wait indefinitely. If the function fails with a timeout error code, it means no buffer with received data became available within the specified timeout, but any buffer that becomes available later can still be obtained by calling this function again.

    :param Buffer: A pointer to a variable that receives a pointer to the transport buffer containing the received data.

.. c:function:: LMQAPI LmqRawChannelDisposeTransportBuffer(LMQ_TRANSPORT Transport, PLMQ_TRANSPORTBUFFER* Buffer, BOOL EraseBufferBeforeDisposing)

    Disposes a transport buffer obtained from LmqRawChannelGetTransportBuffer() or LmqRawChannelReceiveTransportBuffer() that is no longer needed and will never be sent.

    :param Transport: The transport the buffer belongs to.

    :param Buffer: A pointer to the transport buffer to dispose.

    :param EraseBufferBeforeDisposing: If TRUE, the buffer content is securely erased by overwriting it with zeros before the buffer is disposed. This is useful when the buffer contains sensitive data that should not remain in memory. Note that enabling this option can have a performance impact, especially for large buffers, as it requires an additional memory operation to overwrite the buffer content.

Posting and Receiving
=====================

.. c:function:: LMQAPI LmqPostMessage(LMQ_SENDQUEUE SendQueue, PLMQ_MESSAGE Message, UINT32 TimeoutMs)

    Queues a message for sending through a communication channel.

    :param SendQueue: The send queue to send the message through.

    :param Message: A pointer to the message to send. After this call, the application can't access the message anymore as the ownership has been transferred to LwMQ, and it will be asynchronously destroyed at some future point. This parameter being a *pointer* to the message, the function will null it before returning on successful execution, preventing any further use.

    :param TimeoutMs: The maximum time to wait for the message to be accepted for sending, in milliseconds. A value of INFINITE can be used to wait indefinitely. This parameter has no effect for unbounded send queues and must be zero, as they are always ready to accept messages for sending.

.. code:: cpp
    
    HRESULT hr = LmqPostMessage(SendQueue,
                                &Message,
                                1000); // Wait up to 1 second.    

.. c:function:: LMQAPI LmqPostMessageWithTag(LMQ_SENDQUEUE SendQueue, PLMQ_MESSAGE Message, LONG_PTR Tag, UINT32 TimeoutMs)
    
    Queues a message for sending through a communication channel with an associated user-defined tag.

    :param SendQueue: The tagged send queue to send the message through.

    :param Message: A pointer to the message to send. After this call, the application can't access the message anymore as the ownership has been transferred to LwMQ, and it will be asynchronously destroyed at some future point. This parameter being a *pointer* to the message, the function will null it before returning on successful execution, preventing any further use.

    :param Tag: A user-defined tag value that is associated with the message. This value is interpreted by LwMQ while queueing the message, and any message with the same tag still in the queue at the time of insertion is removed before the new message is queued. A value of 0 means "no tag" and the message is queued normally in FIFO order.

    :param TimeoutMs: The maximum time to wait for the message to be accepted for sending, in milliseconds. A value of INFINITE can be used to wait indefinitely. This parameter has no effect for unbounded send queues and must be zero, as they are always ready to accept messages for sending.

    .. note:: This function is useful for sending messages that supersede previous messages with the same tag, for example progress messages where only the latest message is relevant. By using the same tag for all progress messages, you can ensure that only the latest progress message is queued for sending, and any previous progress messages still in the queue are removed, which helps reduce unnecessary message processing on the receiving side. Note, however, that the tagged queues inccurs some overhead as they keep track of the tagged items in the queue for fast access. The coalescing only happen within messages that are qued but not already sent. Sent messages (i.e. mesages that have left, or about to leave in a trnasmission buffer) will not be rescinded. The tag is not transmitted and is only a locally processed artefact. The send queue must be one of the tagged queues: LMQ_SENDQUEUETYPE_MULTIPRODUCER_UNBOUNDED_TAGGED, LMQ_SENDQUEUETYPE_MULTIPRODUCER_BOUNDED_TAGGED_DISCARD_OLDEST, LMQ_SENDQUEUETYPE_MULTIPRODUCER_BOUNDED_TAGGED_DISCARD_NEWEST, otherwise the function will fail with an error.

.. code:: cpp
    
    HRESULT hr = LmqPostMessageWithTag(SendQueue,
                                       &Message,
                                       ProgressMessageTag, // Coalesce per tag.
                                       1000); // Wait up to 1 second.

.. c:function:: LMQAPI LmqReceiveMessage(LMQ_CHANNEL Channel, UINT32 TimeoutMs, PUSHORT FrameCount, PUINT64 PayloadSizeBytes, PLMQ_MESSAGE Message)

    Receives a message from a communication channel.

    :param Channel: The communication channel to receive the message from.

    :param TimeoutMs: The maximum time to wait for a message to be received, in milliseconds. A value of INFINITE can be used to wait indefinitely. All types of receive queues are waitable. Use a value of zero to return immediately (with an error) on an empty queue.

    :param FrameCount: A pointer to a variable that receives the number of frames in the received message if successful. If the function fails, the value is set to zero.

    :param PayloadSizeBytes: An optional pointer to a variable that receives the total size of all frame payloads in bytes in the received message. If the function fails and this pointer was supplied, the value is set to zero.

    :param Message: A pointer to a variable that receives the received message instance on success. The application takes ownership of the received message and is responsible for destroying it when it is no longer needed by calling LmqDisposeReceivedMessage().

.. code:: cpp

    USHORT FrameCount;
    LMQ_MESSAGE Message;
    UINT64 PayloadSizeBytes;

    HRESULT hr = LmqReceiveMessage(Channel,
                                   INFINITE, // Wait indefinitely.
                                   &FrameCount,
                                   &PayloadSizeBytes,
                                   &Message);    

.. c:function:: LMQAPI LmqDisposeReceivedMessage(PLMQ_MESSAGE Message)

    Disposes a received message that is no longer needed.

    :param Message: A pointer to the received message to dispose. The pointer is set to NULL before the function returns.

.. code:: cpp

    HRESULT hr = LmqDisposeReceivedMessage(&Message);
