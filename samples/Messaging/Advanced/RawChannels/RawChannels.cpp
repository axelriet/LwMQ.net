/*++

Copyright (c) Axel Rietschin Software Development, LLC

Module Name:

    RawChannels.cpp

Abstract:

    Simplistic peer-to-peer chat to test LwMQ's
    basic functionality with with raw channel
    buffer. The latency is printed.

Prerequisites:

    The LwMQ SDK must be installed prior to building
    this sample. Get it from www.lwmq.net and make
    sure to restart VS (if it was running) to catch
    the environment variables to LwMQ's inc and lib.

    The SDK version must be 1.0.0.7 or later.

Author:

    Axel Rietschin (26-Mar-2026)

Environment:

    User Mode.

--*/

#define USE_PRECISE_BUT_SLOWER_TIMESTAMPS

#include <Windows.h>

#include <io.h>
#include <fcntl.h>
#include <process.h>

#include <api-lwmq-time.h>
#include <api-lwmq-messaging.h>

#include <api-lwmq-samples-common.h>

#pragma warning(disable: 26481) // Don't use pointer arithmetic.

HRESULT
SendOneBuffer (
    _In_ LMQ_TRANSPORT Transport,
    _In_ PCWSTR MessageContent,
    _In_ SIZE_T MessageSizeBytes
    );

HRESULT
ReceiveOneBuffer (
    _In_ LMQ_TRANSPORT Transport,
    _In_ BOOL PrintData
    ) noexcept;

VOID
CDECL
SenderThread (
    PVOID Param
    ) noexcept;

double g_TimeAdjustmentNs{};

int main()
{
    printf("RawChannels IPC 1.0 - Account must have SeCreateGlobalPrivilege!\n"
           "Start two instances of RawChannels and start typing or pasting text.\n");

#ifdef USE_PRECISE_BUT_SLOWER_TIMESTAMPS

    //
    // Time the time function so we can subtract it
    // from the observed latency.
    //

    const ULONGLONG Start{ RtlGetSystemTimePrecise() };

    for (int x = 0; x < 100'000; x++)
    {
        RtlGetSystemTimePrecise();
        RtlGetSystemTimePrecise();
        RtlGetSystemTimePrecise();
        RtlGetSystemTimePrecise();
        RtlGetSystemTimePrecise();
        RtlGetSystemTimePrecise();
        RtlGetSystemTimePrecise();
        RtlGetSystemTimePrecise();
        RtlGetSystemTimePrecise();
        RtlGetSystemTimePrecise();
    }

    //
    // The system time is in 100ns increments.
    //

    const ULONGLONG Elapsed100Ns{ (RtlGetSystemTimePrecise() - Start) };

    //
    // Compute the time it takes to call the time function
    // once. We did 1M calls, and the result is in 100ns
    // increments, so we multiply my 100 to get nanoseconds,
    // then divide by 1 million (so we divide by 10,000)
    //

    g_TimeAdjustmentNs = Elapsed100Ns / 10'000.0;

    printf("Using precise timestamps with a time adjustment of %fns.\n", g_TimeAdjustmentNs);

#endif

    //
    // Set up a bidirectional channel
    // with a transport and a send queue.
    //

    LMQ_CHANNEL Channel{};
    LMQ_TRANSPORT Transport{};
    CHECK(LmqCreateChannel(LMQ_CHANNELTYPE_RAW_ONE_TO_ONE, // SDK 1.0.0.7 or later
                           &Channel));

    CHECK(LmqAddTransport(Channel,
                          L"ipc://RawChannels-v1",
                          64 * 1024,
                          4,
                          4,
                          LMQ_TRANSPORT_CREATIONFLAGS_SENDRECEIVE,
                          &Transport));

    //
    // Since we don't want any config or hint try opening
    // the IPC channel as a server. If that fails, open as
    // a client. This is not a good general strategy as the
    // server open can fail for many reasons.
    //

    if (FAILED(LmqOpenChannel(Channel,
                              LMQ_CHANNELROLE_SERVER,
                              LMQ_RECEIVEQUEUETYPE_NA,
                              LMQ_QUEUECAPACITY_NA)))
    {
        CHECK(LmqOpenChannel(Channel,
                             LMQ_CHANNELROLE_CLIENT,
                             LMQ_RECEIVEQUEUETYPE_NA,
                             LMQ_QUEUECAPACITY_NA));
    }

    //
    // So far, so good. Technically we can send and
    // receive now, so let's send something and wait
    // for an answer as a first step.
    //

    CHECK(SendOneBuffer(Transport,
                        L"Hello",
                        10));

    CHECK(ReceiveOneBuffer(Transport,
                           FALSE));

    //
    // The channel looks like it's open for
    // business! We can pass our send queue to
    // a separate thread that handles input, and
    // we simply loop here and print anything we
    // receive.
    //

    CloseHandle(reinterpret_cast<HANDLE>(_beginthread(SenderThread,
                                                      0,
                                                      Transport)));

    while (TRUE)
    {
        CHECK(ReceiveOneBuffer(Transport,
                               TRUE));
    }

    //
    // In a real program we'd invent a special
    // message, say "EXIT" that would govern the
    // process to exit the loop, signal the sender
    // thread to exit, and close the channel.
    //

    return 0;
}

HRESULT
SendOneBuffer (
    _In_ LMQ_TRANSPORT Transport,
    _In_ PCWSTR MessageContent,
    _In_ SIZE_T MessageSizeBytes
    )
{
    //
    // Get a buffer directly from the transport.
    //

    PLMQ_TRANSPORTBUFFER TransportBuffer{};

    CHECK(LmqRawChannelGetBuffer(Transport,
                                 INFINITE,
                                 &TransportBuffer));

    _Analysis_assume_(TransportBuffer != nullptr);

    if (MessageSizeBytes > TransportBuffer->BufferSizeBytes)
    {
        CHECK(HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER));
    }

    //
    // Reserve 8 bytes at the front for the timestamp.
    //

    _ASSERT(TransportBuffer->BufferSizeBytes >= 10);

    CHECK(StringCchCopyW(reinterpret_cast<WCHAR*>(&TransportBuffer->Buffer[8]),
                         (TransportBuffer->BufferSizeBytes - 8) / sizeof(WCHAR),
                         MessageContent));

    //
    // Plant the 8-bytes timestamp at the start of the buffer.
    //

#ifdef USE_PRECISE_BUT_SLOWER_TIMESTAMPS 
    *reinterpret_cast<ULONGLONG*>(&TransportBuffer->Buffer[0]) = RtlGetSystemTimePrecise();
#else
    *reinterpret_cast<ULONGLONG*>(&TransportBuffer->Buffer[0]) = RtlGetSystemTime();
#endif

    //
    // Sent the buffer on its way to the other peer. Note
    // that we don't set the length of the data (which is
    // 8 + (wcslen(TransportBuffer->Buffer) + 1) * sizeof(WCHAR)
    // because the string happens to me zero-terminated and
    // we just print it as-is on the other side. In most
    // scenarios you want to pass the count of bytes you
    // actually used via TransportBuffer->BufferSizeBytes.
    //

    CHECK(LmqRawChannelSendBuffer(Transport,
                                    &TransportBuffer,
                                    FALSE));

    return S_OK;
}

HRESULT
ReceiveOneBuffer (
    _In_ LMQ_TRANSPORT Transport,
    _In_ BOOL PrintData
    ) noexcept
{
    PLMQ_TRANSPORTBUFFER TransportBuffer{};

    CHECK_RETURN(LmqRawChannelReceiveBuffer(Transport,
                                            INFINITE,
                                            &TransportBuffer));

    _Analysis_assume_(TransportBuffer != nullptr);

#ifdef USE_PRECISE_BUT_SLOWER_TIMESTAMPS 
    const ULONGLONG Now{ RtlGetSystemTimePrecise() };
#else
    const ULONGLONG Now{ LmqGetSystemTime() };
#endif

    if (PrintData)
    {
        //
        // We know the start of the buffer is a timestamp
        // because this is how this app sends data through
        // a raw channel.
        //

        const auto ElapsedNs{ ((Now - *reinterpret_cast<ULONGLONG*>(&TransportBuffer->Buffer[0])) * 100ULL) }; // Convert to ns

        //
        // We know also know, because again this is how this
        // particular app packs data, that the text starts
        // at offset 8, and is a Unicode zstring with a crlf
        // at the end, so we just print it as is.
        //

        wprintf(L"%8.1fus - %ls",
                (ElapsedNs - g_TimeAdjustmentNs) / 1000.0,
                reinterpret_cast<PCWSTR>(&TransportBuffer->Buffer[8]));
    }

    CHECK_RETURN(LmqRawChannelDisposeBuffer(Transport,
                                            &TransportBuffer,
                                            FALSE));

    return S_OK;
}

VOID
CDECL
SenderThread (
    PVOID Param
    ) noexcept
{
    LMQ_TRANSPORT Transport{ static_cast<LMQ_TRANSPORT>(Param) };

    static_cast<void>(_setmode(_fileno(stdin), _O_U16TEXT));

    //
    // Subtle cue saying we are connected.
    //

    printf("\n");

    while (TRUE)
    {
        //
        // Get a buffer directly from the transport.
        //

        PLMQ_TRANSPORTBUFFER TransportBuffer{};

        CHECK(LmqRawChannelGetBuffer(Transport,
                                     INFINITE,
                                     &TransportBuffer));

        _Analysis_assume_(TransportBuffer != nullptr);

        //
        // Reserve 8 bytes at the front for the timestamp.
        //

        _ASSERT(TransportBuffer->BufferSizeBytes >= 10);

        if (fgetws(reinterpret_cast<WCHAR*>(&TransportBuffer->Buffer[8]),
                   __pragma(warning(suppress:4244)) (TransportBuffer->BufferSizeBytes - 8) / sizeof(WCHAR),
                   stdin) != nullptr)
        {
            //
            // Plant the 8-bytes timestamp at the start of the buffer.
            //

#ifdef USE_PRECISE_BUT_SLOWER_TIMESTAMPS 
            *reinterpret_cast<ULONGLONG*>(&TransportBuffer->Buffer[0]) = RtlGetSystemTimePrecise();
#else
            *reinterpret_cast<ULONGLONG*>(&TransportBuffer->Buffer[0]) = RtlGetSystemTime();
#endif
            //
            // Sent the buffer on its way to the other peer. Note
            // that we don't set the length of the data (which is
            // 8 + (wcslen(TransportBuffer->Buffer) + 1) * sizeof(WCHAR)
            // because the string happens to me zero-terminated and
            // we just print it as-is on the other side. In most
            // scenarios you want to pass the count of bytes you
            // actually used via TransportBuffer->BufferSizeBytes.
            //

            CHECK(LmqRawChannelSendBuffer(Transport,
                                          &TransportBuffer,
                                          FALSE));
        }
        else
        {
            printf("Error reading input.\n");
        }
    }
}
