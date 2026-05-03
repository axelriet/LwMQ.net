/*++

Copyright (c) Axel Rietschin Software Development, LLC

Module Name:

    SegCache.cpp

Abstract:

    LwMQ Segmented In-Memory LRU cache demo.

Prerequisites:

    The LwMQ SDK must be installed prior to building
    this sample. Get it from www.lwmq.net and make
    sure to restart VS (if it was running) to catch
    the environment variables to LwMQ's inc and lib.

Author:

    Axel Rietschin (02-May-2026)

Environment:

    User Mode.

--*/

#include <Windows.h>
#include <process.h>

#include <format>
#include <algorithm>

#include <api-lwmq-time.h>
#include <api-lwmq-segmented-cache.h>

#include <api-lwmq-samples-common.h>

// #define TINY_PAYLOAD     // Forces small payload

#define CACHE_SLOTS      (1'024 * 1'024)
#define CACHE_SEGMENTS   (1'024) // [1, 2, 4, 8, ..., 1024]
#define WORKER_THREADS   (32)
#define ITEMS_PER_THREAD (CACHE_SLOTS / WORKER_THREADS)

//
// WORKER_THREADS must divide CACHE_SLOTS in this demo.
//

static_assert(CACHE_SLOTS % WORKER_THREADS == 0);

CHAR PayloadText[] = "Lorem ipsum dolor sit amet non."
#ifndef TINY_PAYLOAD
                     " Suspendisse maximus vel odio quis ultrices. Sed bibendum "
                     "fringilla sem sit amet porta. Donec tristique tortor cursus, "
                     "ultrices turpis a, finibus tellus. Donec at velit fringilla, "
                     "volutpat nulla ut, ullamcorper risus. Maecenas non tempor "
                     "sapien, feugiat consectetur elit. Nunc sed nibh sed velit "
                     "molestie hendrerit. Sed sollicitudin, ligula vel porta fermentum, "
                     "tortor enim tempor leo, vitae convallis nulla nunc ac ante. Nam "
                     "molestie leo et urna posuere porttitor. Morbi non tincidunt purus, "
                     "quis ultricies neque. Cras sed varius lectus. Morbi dictum risus "
                     "ac neque finibus, vel iaculis odio rhoncus. Vivamus euismod neque "
                     "massa, at efficitur sem sollicitudin id. Integer iaculis odio sed "
                     "diam dignissim, a pulvinar purus scelerisque. Mauris a interdum "
                     "arcu, vitae sodales arcu. Integer ac purus venenatis odio rhoncus "
                     "interdum. Cras pretium maximus dignissim. Nulla sed diam ut risus "
                     "tristique varius at in augue. Mauris nec lectus a est sollicitudin "
                     "imperdiet vel. Maecenas scelerisque eu."
#endif
    ;

HANDLE StartEvent;

LMQ_KEY Key{};
LMQ_SEGMENTEDCACHE Cache{};

unsigned
CALLBACK
InsertThread (
    PVOID Context
    ) noexcept
{
    //
    // Grab a local copy of the key.
    //

    LMQ_KEY LocalKey{ Key };

    //
    // Wait for the start event.
    //

    if (WaitForSingleObject(StartEvent,
                            INFINITE) != WAIT_OBJECT_0)
    {
        CHECK(E_UNEXPECTED);
    }

    //
    // Insert items.
    //

    const SIZE_T StartIndex = reinterpret_cast<UINT_PTR>(Context) * ITEMS_PER_THREAD;

    for (SIZE_T Index = StartIndex; Index < (StartIndex + ITEMS_PER_THREAD); Index++)
    {
        LocalKey.QWord0 = Index;

        CHECK(LmqAddSegmentedCacheEntry(Cache,
                                        &LocalKey,
                                        &PayloadText[0],
                                        sizeof(PayloadText),
                                        LMQ_CACHEENTRY_NO_ADDITIONAL_ENTROPY,
                                        LMQ_CACHEENTRY_FLAGS_NONE,
                                        0.0f));
    }

    return 0;
}

unsigned
CALLBACK
QueryThread (
    PVOID Context
    ) noexcept
{
    //
    // Grab a local copy of the key.
    //

    LMQ_KEY LocalKey{ Key };

    //
    // Wait for the start event.
    //

    if (WaitForSingleObject(StartEvent,
                            INFINITE) != WAIT_OBJECT_0)
    {
        CHECK(E_UNEXPECTED);
    }

    //
    // Retrieve items.
    //

    const SIZE_T StartIndex = reinterpret_cast<UINT_PTR>(Context) * ITEMS_PER_THREAD;

    for (SIZE_T Index = StartIndex; Index < (StartIndex + ITEMS_PER_THREAD); Index++)
    {
        LocalKey.QWord0 = Index;

        SIZE_T DataSize{ sizeof(PayloadText) };
        BYTE RetrievedText[sizeof(PayloadText)];

        CHECK(LmqRetrieveSegmentedCacheEntry(Cache,
                                             &LocalKey,
                                             &RetrievedText[0],
                                             &DataSize,
                                             LMQ_CACHEENTRY_NO_ADDITIONAL_ENTROPY,
                                             nullptr));
    }

    return 0;
}

int main()
{
    std::locale::global(std::locale("en_US.UTF-8"));

    printf("SegCache 1.0 - LwMQ Segmented Cache Demo\n%s-way Segmented Cache, %s-byte entries.\n%s slots/insertions/retrievals with %s threads.\n",
           std::format("{:L}", CACHE_SEGMENTS).c_str(),
           std::format("{:L}", sizeof(PayloadText)).c_str(),
           std::format("{:L}", CACHE_SLOTS).c_str(),
           std::format("{:L}", WORKER_THREADS).c_str());

    //
    // Create a key that we'll modify later.
    //

    CHECK(LmqMakeRfc4122Key(&Key));

    //
    // Create an event to synchronize the worker's start.
    //

    StartEvent = CreateEvent(nullptr,
                             TRUE,
                             FALSE,
                             nullptr);

    if (!StartEvent)
    {
        CHECK(E_UNEXPECTED);
    }

    //
    // Create the segmented LRU data cache.
    //

    constexpr LMQ_CACHEPARAMETERS Parameters
    {
        sizeof(LMQ_CACHEPARAMETERS),
        LMQ_CACHETYPE_DATA_CACHE,
        LMQ_CACHEFLAGS_NONE,
        CACHE_SLOTS
    };

    CHECK(LmqCreateSegmentedCache(&Parameters,
                                  LMQ_SEGMENTEDCACHE_KEYTYPE_RFC4122,
                                  0,
                                  CACHE_SEGMENTS,
                                  &Cache));

    //
    // Populate the cache.
    //

    SIZE_T ThreadIndex{};
    HANDLE Threads[WORKER_THREADS];

    std::ranges::for_each(Threads, [&ThreadIndex](HANDLE& Thread) noexcept
    {
        Thread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr,
                                                         0,
                                                         InsertThread,
                                                         reinterpret_cast<PVOID>(ThreadIndex++),
                                                         0,
                                                         nullptr));
    });

    //
    // Add entries.
    //

    printf("\nInserting %s entries using %d threads.\n",
           std::format("{:L}", CACHE_SLOTS).c_str(),
           WORKER_THREADS);

    UINT64 StartNs = LmqGetTickCountNs();

    SetEvent(StartEvent);

    WaitForMultipleObjects(_countof(Threads),
                           &Threads[0],
                           TRUE,
                           INFINITE);

    UINT64 ElapsedNs = LmqTimeElapsedNsSince(StartNs);

    std::ranges::for_each(Threads, [](HANDLE& Thread) noexcept
    {
        CloseHandle(Thread);
    });

    ResetEvent(StartEvent);

    double Throughput = ((double)CACHE_SLOTS / ElapsedNs * 1'000'000'000.0);

    printf("Elapsed: %s [ms] @ %s multi-threaded insertion/sec.\n\n",
           std::format("{:.0Lf}", ElapsedNs / 1'000'000.0).c_str(),
           std::format("{:.0Lf}", Throughput).c_str());

    //
    // Retrieve entries.
    //

    ThreadIndex = 0;

    std::ranges::for_each(Threads, [&ThreadIndex](HANDLE& Thread) noexcept
    {
        Thread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr,
                                                         0,
                                                         QueryThread,
                                                         reinterpret_cast<PVOID>(ThreadIndex++),
                                                         0,
                                                         nullptr));
    });

    printf("Retrieving %s x 1KB entries using %d threads.\n",
           std::format("{:L}", CACHE_SLOTS).c_str(),
           WORKER_THREADS);

    StartNs = LmqGetTickCountNs();

    SetEvent(StartEvent);

    WaitForMultipleObjects(_countof(Threads),
                           &Threads[0],
                           TRUE,
                           INFINITE);

    ElapsedNs = LmqTimeElapsedNsSince(StartNs);

    Throughput = ((double)CACHE_SLOTS / ElapsedNs * 1'000'000'000.0);

    printf("Elapsed: %s [ms] @ %s multi-threaded retrieval/sec (%s-byte items @ %.1f GB/sec)\n",
            std::format("{:.0Lf}", ElapsedNs / 1'000'000.0).c_str(),
            std::format("{:.0Lf}", Throughput).c_str(),
            std::format("{:L}", sizeof(PayloadText)).c_str(),
            (Throughput * sizeof(PayloadText)) / (1024.0 * 1024.0 * 1024.0));

    //
    // Cleanup.
    //

    std::ranges::for_each(Threads, [](HANDLE& Thread) noexcept
    {
        CloseHandle(Thread);
    });

    CloseHandle(StartEvent);

    //
    // Done, destroy segmented cache.
    //

    CHECK(LmqDestroySegmentedCache(&Cache));

    return 0;
}
