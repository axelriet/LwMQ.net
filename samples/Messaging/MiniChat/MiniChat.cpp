/*++

Copyright (c) Axel Rietschin Software Development, LLC

Module Name:

    MiniChat.cpp

Abstract:

    Simplistic peer-to-peer chat to test LwMQ's
    basic functionality.

Author:

    Axel Rietschin (22-Mar-2026)

Environment:

    User Mode.

--*/

#include <Windows.h>
#include <api-lwmq-messaging.h>

#define CHECK(__hr__) if (FAILED(__hr__)) { printf("Something went wrong: 0x%08lx\r", __hr__); exit(-1); }

int main()
{
    printf("MiniChat 1.0\n");

    //
    // Set up a bidirectional channel
    // with a transport and a send queue.
    //

    LMQ_CHANNEL Channel;
    
    CHECK(LmqCreateChannel(LMQ_CHANNELTYPE_ONE_TO_ONE,
                           &Channel));

    CHECK(LmqAddTransport(Channel,
                          L"ipc://MiniChat-v1",
                          16 * 1024,
                          2,
                          2,
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
    // a client.
    //

    if (FAILED(LmqOpenChannel(Channel,
                              LMQ_CHANNELROLE_SERVER,
                              LMQ_RECEIVEQUEUETYPE_MONOCONSUMER_UNBOUNDED,
                              LMQ_QUEUECAPACITY_UNBOUNDED)))
    {
        CHECK(LmqOpenChannel(Channel,
                             LMQ_CHANNELROLE_CLIENT,
                             LMQ_RECEIVEQUEUETYPE_MONOCONSUMER_UNBOUNDED,
                             LMQ_QUEUECAPACITY_UNBOUNDED))
    }
}
