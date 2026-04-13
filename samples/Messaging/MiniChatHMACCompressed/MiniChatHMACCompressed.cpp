/*++

Copyright (c) Axel Rietschin Software Development, LLC

Module Name:

    MiniChatHMACCompressed.cpp

Abstract:

    Simplistic peer-to-peer chat to demonstrate LwMQ's
    messaging functionality with message authentication
    codes prepended in an dedicated message frame.

Prerequisites:

    The LwMQ SDK must be installed prior to building
    this sample. Get it from www.lwmq.net and make
    sure to restart VS (if it was running) to catch
    the environment variables to LwMQ's inc and lib.

Author:

    Axel Rietschin (8-Apr-2026)

Environment:

    User Mode.

--*/

#include <Windows.h>

#include <io.h>
#include <fcntl.h>
#include <process.h>

#include <api-lwmq-time.h>
#include <api-lwmq-hash.h>
#include <api-lwmq-storage.h>
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

//
// Not the best example. Password storage and retrieval
// is out of scope for this sample, but in any case never
// store the password in the code!
//

static const CHAR g_SecretPassword[] = { "Password" };

//
// The HMAC key is encrypted at rest in this process.
//

static LMQ_KEY g_HmacKey;

int main()
{
    printf("MiniChatHMAC (Compressed) IPC 1.0 - Account must have SeCreateGlobalPrivilege!\n"
           "Start two instances of MiniChatHMACCompressed and start typing or pasting text.\n");

    //
    // Derive a key from the password and protect it.
    //

    static_assert(sizeof(g_SecretPassword) > 1);

    CHECK(LmqKeyFromStringA(&g_SecretPassword[0],
                            sizeof(g_SecretPassword),
                            &g_HmacKey));

    CHECK(LmqProtectKey(&g_HmacKey));

    //
    // Set up a bidirectional channel
    // with a transport and a send queue.
    //

    LMQ_CHANNEL Channel;
    
    CHECK(LmqCreateChannel(LMQ_CHANNELTYPE_ONE_TO_ONE,
                           &Channel));

    CHECK(LmqAddTransport(Channel,
                          L"ipc://MiniChatHMACCompressed-v1",
                          64 * 1024,
                          4,
                          4,
                          LMQ_TRANSPORT_CREATIONFLAGS_SENDRECEIVE,
                          nullptr));

    LMQ_SENDQUEUE SendQueue;

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
    LMQ_HMAC Hmac{};
    LMQ_MESSAGE Message{};

    //
    // Compute the message authentication code.
    //

    CHECK_RETURN(LmqComputeHMAC(MessagePayload,
                                MessagePayloadSizeBytes,
                                &g_HmacKey,
                                TRUE,
                                &Hmac));

    //
    // Create a message, hinting about 2 frames.
    //

    CHECK_RETURN(LmqCreateMessage(2,
                                  &Message));

    //
    // By convention for this app, put the HMAC in frame 0.
    //

    CHECK_RETURN(LmqAppendFrame(Message,
                                &Hmac,
                                sizeof(Hmac),
                                LMQ_TIMESTAMP_NONE));

    //
    // By convention for this app, put the payload in frame 1.
    //

    SIZE_T CompressedDataBlobSize;
    LMQ_COMPRESSEDDATABLOB CompressedBlob;

    CHECK_RETURN(LmqCompressData(MessagePayload,
                                 MessagePayloadSizeBytes,
                                 &CompressedBlob,
                                 &CompressedDataBlobSize));

    CHECK_RETURN(LmqAppendFrame(Message,
                                CompressedBlob,
                                CompressedDataBlobSize,
                                Timestamp));

    CHECK_RETURN(LmqFreeCompressedDataBlob(&CompressedBlob));

    //
    // Post the message.
    //

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
ReceiveOneMessage (
    _In_ LMQ_CHANNEL Channel,
    _In_ SIZE_T ExpectedPayloadSizeBytes,
    _In_ BOOL PrintData
    ) noexcept
{
    PCLMQ_HMAC Hmac{};
    const BYTE* Data{};
    USHORT FrameCount{};
    LMQ_MESSAGE Message{};
    SIZE_T DataSizeBytes{};

    CHECK_RETURN(LmqReceiveMessage(Channel,
                                   INFINITE,
                                   &FrameCount,
                                   nullptr,
                                   &Message));

    if (FrameCount != 2)
    {
        printf("The message has an unexpected number of frames: %d, should be 2 for this app.\n", FrameCount);

        return E_FAIL;
    }

    //
    // By convention for this app, the HMAC is in frame 0.
    //

    CHECK_RETURN(LmqGetFrameData(Message,
                                 0,
                                 PPCVOID(&Hmac),
                                 &DataSizeBytes,
                                 nullptr,
                                 nullptr));

    if (DataSizeBytes != sizeof(LMQ_HMAC))
    {
        printf("Frame 0 has an unexpected size, should be %zu for this app.\n", sizeof(LMQ_HMAC));

        return E_FAIL;
    }

    //
    // By convention for this app, the payload is in frame 1.
    //

    CHECK_RETURN(LmqGetFrameData(Message,
                                 1,
                                 PPCVOID(&Data),
                                 &DataSizeBytes,
                                 nullptr,
                                 nullptr));

    static WCHAR Buffer[63 * 1024]{};
    SIZE_T UncompressedDataSizeBytes{ sizeof (Buffer) };

    CHECK_RETURN(LmqDecompressData(LmqCompressedDataBlobFromPointer(Data),
                                   &Buffer[0],
                                   &UncompressedDataSizeBytes));

    if (ExpectedPayloadSizeBytes && (UncompressedDataSizeBytes != ExpectedPayloadSizeBytes))
    {
        printf("The message has an unexpected payload size: %zu, expected %zu\n", UncompressedDataSizeBytes, ExpectedPayloadSizeBytes);

        return E_FAIL;
    }

    //
    // Verify the message integrity and authenticity.
    //

    CHECK_RETURN(LmqVerifyHMAC(&Buffer[0],
                               UncompressedDataSizeBytes,
                               &g_HmacKey,
                               TRUE,
                               Hmac));

    //
    // The message is authentic, sir! I concur, sir!
    //

    if (PrintData)
    {
        const int Cch{ __pragma(warning(suppress:26472)) static_cast<int>(UncompressedDataSizeBytes / sizeof(WCHAR)) };

        wprintf(L"%.*ls",
                Cch,
                &Buffer[0]);
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
