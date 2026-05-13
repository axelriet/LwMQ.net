/*++

Copyright (c) Axel Rietschin Software Development, LLC

Module Name:

    MiniChatEx.cpp

Abstract:

    Simplistic peer-to-peer chat to demonstrate LwMQ's
    basic functionality with latency printing.

Prerequisites:

    The LwMQ SDK must be installed prior to building
    this sample. Get it from www.lwmq.net and make
    sure to restart VS (if it was running) to catch
    the environment variables to LwMQ's inc and lib.

Author:

    Axel Rietschin (23-Mar-2026)

Environment:

    User Mode.

--*/

#define USE_PRECISE_BUT_SLOWER_TIMESTAMPS

#include <io.h>
#include <fcntl.h>
#include <process.h>

#include <api-lwmq-time.h>
#include <api-lwmq-messaging.h>

#include <api-lwmq-samples-common.h>

HRESULT
PostOneMessage (
    _In_ LMQ_SENDQUEUE SendQueue,
    _In_ PCWSTR MessagePayload,
    _In_ SIZE_T MessagePayloadSizeBytes,
    _In_ ULONG64 Timestamp
    ) noexcept;

HRESULT
ReceiveOneMessage (
    _In_ LMQ_CHANNEL Channel,
    _In_ SIZE_T ExpectedPayloadSizeBytes,
    _In_ BOOL PrintData
    ) noexcept;

VOID
CDECL
SenderThread (
    _In_ PVOID Param
    ) noexcept;

int main()
{
    printf("MiniChatEx IPC 1.0 - Account must have SeCreateGlobalPrivilege!\n"
           "Start two instances of MiniChatEx and start typing or pasting text.\n");

#ifdef USE_PRECISE_BUT_SLOWER_TIMESTAMPS

    g_TimingAdjustmentNs = ComputeTimingOverhead();

    printf("Using precise timestamps with a time adjustment of %.0fns.\n\n", g_TimingAdjustmentNs);

#endif

    printf("*** IF YOU GET AN ACCESS DENIED ERROR (0x8007005) CHECK THE ACCOUNT PRIVILEGES ***\n\n");

    //
    // Set up a bidirectional channel
    // with a transport and a send queue.
    //

    LMQ_CHANNEL Channel{};
    
    CHECK(LmqCreateChannel(LMQ_CHANNELTYPE_ONE_TO_ONE,
                           &Channel));

    CHECK(LmqAddTransport(Channel,
                          L"ipc://MiniChatEx-v1",
                          64 * 1024,
                          4,
                          4,
                          LMQ_TRANSPORTCREATIONFLAGS_SENDRECEIVE,
                          nullptr));

    LMQ_SENDQUEUE SendQueue{};

    CHECK(LmqAddSendQueue(Channel,
                          LMQ_SENDQUEUETYPE_MONOPRODUCER_UNBOUNDED,
                          LMQ_SENDQUEUEPRIORITY_NORMAL,
                          LMQ_QUEUECAPACITY_UNBOUNDED,
                          &SendQueue));

    //
    // Since we don't want any config or hint try opening
    // the IPC channel as a server. If that fails, open as
    // a client. This is not a good general strategy as the
    // server open can fail for many reasons.
    //

    if (FAILED(LmqOpenChannel(Channel,
                              LMQ_CHANNELROLE_SERVER,
                              LMQ_RECEIVEQUEUETYPE_MONOCONSUMER_UNBOUNDED,
                              LMQ_QUEUECAPACITY_UNBOUNDED)))
    {
        CHECK(LmqOpenChannel(Channel,
                             LMQ_CHANNELROLE_CLIENT,
                             LMQ_RECEIVEQUEUETYPE_MONOCONSUMER_UNBOUNDED,
                             LMQ_QUEUECAPACITY_UNBOUNDED));
    }

    //
    // So far, so good. Technically we can send and
    // receive now, so let's send something and wait
    // for an answer as a first step.
    //

    CHECK(PostOneMessage(SendQueue,
                         L"Hello",
                         10,
                         LMQ_TIMESTAMP_NONE));

    CHECK(ReceiveOneMessage(Channel,
                            10,
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
                                                      SendQueue)));

    while (TRUE)
    {
        CHECK(ReceiveOneMessage(Channel,
                                0,
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
PostOneMessage (
    _In_ LMQ_SENDQUEUE SendQueue,
    _In_ PCWSTR MessagePayload,
    _In_ SIZE_T MessagePayloadSizeBytes,
    _In_ ULONG64 Timestamp
    ) noexcept
{
    HRESULT hr{};
    LMQ_MESSAGE Message{};

    CHECK_RETURN(LmqCreateMessage(LMQ_MESSAGEFRAMECOUNT_DEFAULT,
                                  &Message));

    CHECK_RETURN(LmqAppendFrame(Message,
                                reinterpret_cast<const BYTE*>(MessagePayload),
                                MessagePayloadSizeBytes,
                                Timestamp));

    if (FAILED(hr = LmqPostMessage(SendQueue,
                                   &Message,
                                   0)))
    {
        //
        // Messages that were never sent must be
        // destroyed.
        //

        CHECK_RETURN(LmqDestroyUnpostedMessage(&Message));

        CHECK_RETURN(hr);
    }

    return S_OK;
}

HRESULT
ReceiveOneMessage(
    _In_ LMQ_CHANNEL Channel,
    _In_ SIZE_T ExpectedPayloadSizeBytes,
    _In_ BOOL PrintData
) noexcept
{
    USHORT FrameCount{};
    LMQ_MESSAGE Message{};
    UINT64 PayloadSizeBytes{};

    CHECK_RETURN(LmqReceiveMessage(Channel,
                                   INFINITE,
                                   &FrameCount,
                                   &PayloadSizeBytes,
                                   &Message));

#ifdef USE_PRECISE_BUT_SLOWER_TIMESTAMPS 
    const ULONGLONG Now{ RtlGetSystemTimePrecise() };
#else
    const ULONGLONG Now{ LmqGetSystemTime() };
#endif

    if (PrintData)
    {
        PCWSTR Data{};
        ULONGLONG Sent{};
        SIZE_T DataSizeBytes{};

        CHECK_RETURN(LmqGetFrameData(Message,
                                     0,
                                     PPCVOID(&Data),
                                     &DataSizeBytes,
                                     &Sent,
                                     nullptr));

        const auto ElapsedNs{ Sent ? ((Now - Sent) * 100ULL) : 0 }; // Convert to ns

        const int Cch{ __pragma(warning(suppress:26472)) static_cast<int>(DataSizeBytes / sizeof(WCHAR)) };

        wprintf(L"%8.1fus - %.*ls",
                (ElapsedNs - g_TimingAdjustmentNs) / 1000.0,
                Cch,
                Data);
    }
    else
    {
        if (FrameCount != 1)
        {
            printf("The message has an unexpected number of frames: %d, should be 1 for this app.\n", FrameCount);

            return E_FAIL;
        }

        if (ExpectedPayloadSizeBytes && (PayloadSizeBytes != ExpectedPayloadSizeBytes))
        {
            printf("The message has an unexpected payload size: %zu, expected %zu\n", PayloadSizeBytes, ExpectedPayloadSizeBytes);

            return E_FAIL;
        }
    }

    CHECK_RETURN(LmqDisposeReceivedMessage(&Message));

    return S_OK;
}

VOID
CDECL
SenderThread (
    _In_ PVOID Param
    ) noexcept
{
    LMQ_SENDQUEUE SendQueue{ static_cast<LMQ_SENDQUEUE>(Param) };

    static_cast<void>(_setmode(_fileno(stdin), _O_U16TEXT));

    //
    // Subtle cue saying we are connected.
    //

    printf("\n");

    while (TRUE)
    {
        static WCHAR Buffer[63 * 1024];

        if (fgetws(&Buffer[0],
                   _countof(Buffer),
                   stdin) != nullptr)
        {
#ifdef USE_PRECISE_BUT_SLOWER_TIMESTAMPS 
            CHECK(PostOneMessage(SendQueue,
                                 &Buffer[0],
                                 sizeof(WCHAR) * wcslen(&Buffer[0]),
                                 LMQ_TIMESTAMP_USE_SYSTEMTIME_PRECISE));
#else
            CHECK(PostOneMessage(SendQueue,
                                 &Buffer[0],
                                 sizeof(WCHAR) * wcslen(&Buffer[0]),
                                 LMQ_TIMESTAMP_USE_SYSTEMTIME));
#endif
        }
        else
        {
            printf("Error reading input.\n");
        }
    }
}
