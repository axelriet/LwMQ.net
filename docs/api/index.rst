.. |us| unicode:: U+00B5 s

*************
API Reference
*************

Initialization and Termination
==============================

Macros
------

.. c:macro:: LMQ_CURRENT_API_VERSION

Functions
---------

.. c:function:: LMQAPI LmqInitialize(WORD VersionRequested)

    Initializes LwMQ.

    :param VersionRequested: Pass the supplied LMQ_CURRENT_API_VERSION macro.

    .. note:: This function must be called before any other LwMQ function. Multiple calls are allowed but must be matched by a corresponding call to LmqTerminate().

.. c:function:: LMQAPI LmqTerminate()

    Shuts down LwMQ and deallocate internal ressources.
    
    .. note:: This function must be called once for each calls made to LmqInitialize().

.. code:: c

    #include <api-lwmq-messaging.h>

    HRESULT hr = LmqInitialize(LMQ_CURRENT_API_VERSION);

    // ...

    hr = LmqTerminate();

Messages
========

Messages are structured entities composed of one or more data frames.

Types
-----

.. c:type:: LMQ_MESSAGE;

    An opaque message instance.

.. c:type:: PLMQ_MESSAGE

    A pointer to an opaque message instance.

Functions
---------

.. c:function:: LMQAPI LmqCreateMessage(USHORT FrameCountHint, PLMQ_MESSAGE Message)

    Creates a new message instance.

    :param FrameCountHint: A hint for the number of frames that will be added to the message. This is used to optimize internal data structures but does not limit the actual number of frames that can be added. For most applications, pass LMQ_MESSAGEFRAMECOUNT_DEFAULT.

    :param Message: A pointer to a variable that receives the created message instance.

    .. note:: The FrameCountHint value is indicative and does not represent a limit. LwMQ may optimize internal allocations knowing the number of frames that will be added to a message, but applications are not required to provide an accurate hint. Extremely performance-sensitive applications benefit from using a single frame, which is the default value used when passing LMQ_MESSAGEFRAMECOUNT_DEFAULT.
        
.. code:: c
    
    LMQ_MESSAGE Message;

    HRESULT hr = LmqCreateMessage(LMQ_MESSAGEFRAMECOUNT_DEFAULT,
                                  &Message);

.. c:function:: LMQAPI LmqCreateMessageRef(LMQ_MESSAGE Message, PLMQ_MESSAGE Clone)

    :param Message: The message to create a reference from.

    :param Clone: A pointer to a variable that receives the created message instance. The clone shares the same data frames as the original message and is independent of the original message's lifetime.

.. code:: c
    
    LMQ_MESSAGE Clone;

    HRESULT hr = LmqCreateMessageRef(Message,
                                     &Clone);

.. c:function:: LMQAPI LmqAppendFrame(LMQ_MESSAGE Message, const BYTE* Data, UINT64 DataSize, ULONG64 Timestamp)

    :param Message: The message to append the frame to.

    :param Data: A pointer to the frame data. The data is copied into the message and can be freed or reused by the caller after the function returns.

    :param DataSize: The size of the frame data in bytes. Note that small payloads are copied "inline" without requiring additional internal memory allocation, therefore improving performance. Use LmqQueryMaxInlineFrameSize() to determine the maximum payload size that can be stored inline, if that is important for your application.

    :param Timestamp: An optional timestamp associated with the frame or one of three special values: LMQ_TIMESTAMP_NONE, LMQ_TIMESTAMP_USE_SYSTEMTIME, suitable for most applications, or LMQ_TIMESTAMP_USE_SYSTEMTIME_PRECISE for applications requiring the the highest possible level of precision (\<1 |us|) at some performance expense.

    .. note:: If supplied explicitly, the timestamp must be specified in Windows NT time format: a 64-bit integer representing the number of 100-nanosecond intervals that have elapsed since January 1, 1601. All timestamps are in UTC format so they are not affected by changes in time zone or daylight saving time.

.. code:: c
    
    HRESULT hr = LmqAppendFrame(Message,
                                (BYTE*)"Hello, World!", // Copied.
                                13,
                                LMQ_TIMESTAMP_NONE);

.. c:function:: LMQAPI LmqAppendStaticFrame(LMQ_MESSAGE Message, const BYTE* Data, UINT64 DataSize, ULONG64 Timestamp)

    :param Message: The message to append the frame to.

    :param Data: A pointer to the frame data. The data is not copied into the message and must remain valid for the lifetime of the communication channel to which the message will be queued, as well as the lifetime of any message references created from the message. Static frames are intended to send static data, for example a static string, or some static binary data outliving the lifespan of the communication channel.

    :param DataSize: The size of the frame data in bytes. The data is not copied into the message.

    :param Timestamp: An optional timestamp associated with the frame. See the Timestamp parameter of LmqAppendFrame() for important details.

.. code:: c
    
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

    :param Context: An optional user-defined context pointer that will be passed to the callback function.

.. code:: c

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
    
    HRESULT hr = LmqAppendExternalFrame(Message,
                                        DataBuffer,
                                        DataBufferSizeBytes,
                                        LMQ_TIMESTAMP_NONE
                                        CallbackFunction,
                                        CallbackContext);

.. c:function:: LMQAPI LmqGetFrameCount(LMQ_MESSAGE Message, PUSHORT FrameCount, PUINT64 PayloadSizeBytes)

    :param Message: The message to query.

    :param FrameCount: A pointer to a variable that receives the number of frames in the message.

    :param PayloadSizeBytes: An optional pointer to a variable that receives the total size of all frame payloads in bytes.

.. code:: c
    
    USHORT FrameCount;
    UINT64 PayloadSizeBytes;

    HRESULT hr = LmqGetFrameCount(Message,
                                  &FrameCount,
                                  &PayloadSizeBytes);

.. c:function:: LMQAPI LmqGetFrameData(LMQ_MESSAGE Message, USHORT FrameIndex, const BYTE** Data, PUINT64 DataSize, PULONG64 Timestamp, PLMQ_MESSAGEFRAMEENUMHINT EnumHint)

    :param Message: The message to query.

    :param FrameIndex: The zero-based index of the frame to query.

    :param Data: A pointer to a variable that receives a pointer to the frame data. The data is owned by the message and must not be modified or freed by the caller.

    :param DataSize: A pointer to a variable that receives the size of the frame data in bytes.

    :param Timestamp: An optional pointer to a variable that receives the timestamp associated with the frame. See the Timestamp parameter of LmqAppendFrame() for important details.

    :param EnumHint: An optional pointer to an enumeration hint structure that can be used to optimize enumeration of multiple frames in a message. If supplied, the caller must initialize the structure's Version field to LMQ_MESSAGEFRAMEENUMHINT_CURRENT_VERSION before the first call to LmqGetFrameData() for a given message, and must not modify any other fields of the structure. The enumeration hint is valid for all subsequent calls to LmqGetFrameData() for the same message until either the message is destroyed or LmqInitFrameEnumHint() is called again for the same enumeration hint structure.

.. code:: c

        const BYTE* Data;
        UINT64 DataSizeBytes;
        ULONG64 Timestamp;
    
        HRESULT hr = LmqGetFrameData(Message,
                                     0,
                                     &Data,
                                     &DataSizeBytes,
                                     &Timestamp,
                                     nullptr);
    
.. c:function:: LMQAPI LmqInitFrameEnumHint(PLMQ_MESSAGEFRAMEENUMHINT EnumHint)

    :param EnumHint: A pointer to an opaque enumeration hint structure to initialize.

.. code:: c

    const BYTE* Data;
    UINT64 DataSizeBytes;
    ULONG64 Timestamp;
    LMQ_MESSAGEFRAMEENUMHINT EnumHint;

    HRESULT hr = LmqInitFrameEnumHint(&EnumHint);

    hr = LmqGetFrameData(Message,
                         0,
                         &Data,
                         &DataSizeBytes,
                         &Timestamp,
                         &EnumHint);

.. note:: The use of an enumeration hint is optional but recommended for performance reasons when enumerating multiple frames in a message in a message with many frames. Moreover, the performance is optimal when accessing frames in increasing order.

.. c:function:: LMQAPI LmqQueryMaxInlineFrameSize(PUSHORT MaxInlineFrameSize)

    :param MaxInlineFrameSize: A pointer to a variable that receives the maximum frame payload size in bytes that can be stored "inline" within the message without requiring additional internal memory allocation.

    .. note:: The value is indicative and does not represent a limit. Application that are extremely performance-sensitive benefit from keeping frame payloads at or below this size, as well as indicating the expected number of frames they intend to add to a message upon message creation.

Queues
======

Transports
==========

Channels
========
