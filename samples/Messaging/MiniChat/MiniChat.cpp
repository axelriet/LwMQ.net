/*++

Copyright (c) Axel Rietschin Software Development, LLC

Module Name:

    MiniChat.cpp

Abstract:

    Simplistic peer-to-peer chat to demonstrate LwMQ's
    basic functionality.

Prerequisites:

    The LwMQ SDK must be installed prior to building
    this sample. Get it from www.lwmq.net and make
    sure to restart VS (if it was running) to catch
    the environment variables to LwMQ's inc and lib.

Author:

    Axel Rietschin (22-Mar-2026)

Environment:

    User Mode.

--*/

#include <Windows.h>

#include <io.h>
#include <fcntl.h>
#include <process.h>

#include <api-lwmq-time.h>
#include <api-lwmq-messaging.h>

#include <api-lwmq-samples-common.h>

HRESULT
PostOneMessage (
    _In_ LMQ_SENDQUEUE SendQueue,
    _In_ PCWSTR MessageContent,
    _In_ SIZE_T MessageSizeBytes,
    _In_ ULONG64 Timestamp
    ) noexcept;

HRESULT
ReceiveOneMessage (
    _In_ LMQ_CHANNEL Channel,
    _In_ int ExpectedPayloadSize,
    _In_ BOOL PrintData
    ) noexcept;

VOID
CDECL
SenderThread (
    PVOID Param
    ) noexcept;

int main()
{
    printf("MiniChat IPC 1.0 - Account must have SeCreateGlobalPrivilege!\n"
           "Start two instances of MiniChat and start typing or pasting text.\n");

    //
    // Set up a bidirectional channel
    // with a transport and a send queue.
    //

    LMQ_CHANNEL Channel{};
    
    CHECK(LmqCreateChannel(LMQ_CHANNELTYPE_ONE_TO_ONE,
                           &Channel));

    CHECK(LmqAddTransport(Channel,
                          L"ipc://MiniChat-v1",
                          64 * 1024,
                          4,
                          4,
                          LMQ_TRANSPORT_CREATIONFLAGS_SENDRECEIVE,
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
    _In_ PCWSTR MessageContent,
    _In_ SIZE_T MessageSizeBytes,
    _In_ ULONG64 Timestamp
    ) noexcept
{
    HRESULT hr{};
    LMQ_MESSAGE Message{};

    CHECK_RETURN(LmqCreateMessage(LMQ_MESSAGEFRAMECOUNT_DEFAULT,
                                  &Message));

    CHECK_RETURN(LmqAppendFrame(Message,
                                reinterpret_cast<const BYTE*>(MessageContent),
                                MessageSizeBytes,
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
    _In_ int ExpectedPayloadSize,
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

    if (PrintData)
    {
        SIZE_T DataSize{};
        const BYTE* Data{};

        CHECK_RETURN(LmqGetFrameData(Message,
                                     0,
                                     &Data,
                                     &DataSize,
                                     nullptr,
                                     nullptr));

        const int Cch{ __pragma(warning(suppress:26472)) static_cast<int>(DataSize / sizeof(WCHAR)) };

        wprintf(L"%.*ls",
                Cch,
                reinterpret_cast<PCWSTR>(Data));
    }
    else
    {
        if (FrameCount != 1)
        {
            printf("The message has an unexpected number of frames: %d, should be 1 for this app.\n", FrameCount);

            return E_FAIL;
        }

        if (ExpectedPayloadSize && (PayloadSizeBytes != ExpectedPayloadSize))
        {
            printf("The message has an unexpected payload size: %zu, expected %d\n", PayloadSizeBytes, ExpectedPayloadSize);

            return E_FAIL;
        }
    }

    CHECK_RETURN(LmqDisposeReceivedMessage(&Message));

    return S_OK;
}

#pragma warning(disable:6262) // Function uses '131092' bytes of stack.

VOID
CDECL
SenderThread (
    PVOID Param
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
        WCHAR Buffer[64 * 1024];

        if (fgetws(&Buffer[0],
                   _countof(Buffer),
                   stdin) != nullptr)
        {
            CHECK(PostOneMessage(SendQueue,
                                 &Buffer[0],
                                 sizeof(WCHAR) * wcslen(&Buffer[0]),
                                 LMQ_TIMESTAMP_NONE));
        }
        else
        {
            printf("Error reading input.\n");
        }
    }
}
