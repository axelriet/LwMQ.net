/*++

Copyright (c) Axel Rietschin Software Development, LLC

Module Name:

    SegCache.cpp

Abstract:

    LwMQ Segemented In-Memory LRU cache demo.

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

#define CACHE_SLOTS      (1'024 * 1'024)
#define CACHE_SEGMENTS   (1'024)
#define QUERY_THREADS    (16)

//
// QUERY_THREADS must divide CACHE_SLOTS in this demo.
//

static_assert(CACHE_SLOTS % QUERY_THREADS == 0);

CHAR PayloadText1024[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
                         "Suspendisse maximus vel odio quis ultrices. Sed bibendum "
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
                         "imperdiet vel.";

HANDLE StartEvent;

LMQ_KEY Key{};
LMQ_SEGMENTEDCACHE Cache{};

static
unsigned
CALLBACK
RetrieveThread (
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

    WaitForSingleObject(StartEvent,
                        INFINITE);

    //
    // Retrieve CACHE_SLOTS / QUERY_THREADS items
    //

    const DWORD StartIndex = (reinterpret_cast<UINT_PTR>(Context) & UINT_MAX);

    for (DWORD Index = StartIndex; Index < (StartIndex + (CACHE_SLOTS / QUERY_THREADS)); Index++)
    {
        LocalKey.DWord0 = Index;

        SIZE_T DataSize{ sizeof(PayloadText1024) };
        BYTE RetrievedText[sizeof(PayloadText1024)];

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

    printf("SegCache 1.0\n1024-way Segmented Cache, 1KB entries.\n1,048,576 slots/inserts, 1,048,576 retrievals with 16 threads.\n");

    //
    // Create a key that we'll modify later.
    //

    CHECK(LmqMakeRfc4122Key(&Key));

    //
    // Create worker threads.
    //

    StartEvent = CreateEvent(nullptr,
                             TRUE,
                             FALSE,
                             nullptr);

    if (!StartEvent)
    {
        CHECK(E_UNEXPECTED);
    }

    HANDLE Threads[QUERY_THREADS];

    std::ranges::for_each(Threads, [](HANDLE& Thread) noexcept
    {
        Thread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr,
                                                         0,
                                                         RetrieveThread,
                                                         nullptr,
                                                         0,
                                                         nullptr));
    });

    //
    // Create a 1024-way segmented LRU data cache with 1,048,576 slots.
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
    // Add 1 million entries. The key is made unique for
    // each entry simply by incrementing some part of it.
    //

    printf("\nInserting 1,048,576 x 1KB entries in the Segmented LRU cache (single-thread).\n");

    UINT64 StartNs = LmqGetTickCountNs();

    for (DWORD Index = 0; Index < CACHE_SLOTS; Index++)
    {
        //
        // We stuff the Index in the key. That makes 1 million
        // unique key although they only differ slightly.
        //

        Key.DWord0 = Index;

        CHECK(LmqAddSegmentedCacheEntry(Cache,
                                        &Key,
                                        &PayloadText1024[0],
                                        sizeof(PayloadText1024),
                                        LMQ_CACHEENTRY_NO_ADDITIONAL_ENTROPY,
                                        LMQ_CACHEENTRY_FLAGS_NONE,
                                        0.0f));
    }

    UINT64 ElapsedNs = LmqTimeElapsedNsSince(StartNs);

    double Throughput = ((double)CACHE_SLOTS / ElapsedNs * 1'000'000'000.0);

    printf("Elapsed: %s [ms] @ %s insertion/sec (1KB items)\n\n",
           std::format("{:.0Lf}", ElapsedNs / 1'000'000.0).c_str(),
           std::format("{:.0Lf}", Throughput).c_str());

    //
    // Retrieve all entries using QUERY_THREADS threads. The timing
    // includes scheduling so the cache itself is faster than measured.
    //

    printf("Retrieving 1,048,576 x 1KB using %d threads.\n", QUERY_THREADS);

    StartNs = LmqGetTickCountNs();

    SetEvent(StartEvent);

    WaitForMultipleObjects(_countof(Threads),
                           &Threads[0],
                           TRUE,
                           INFINITE);

    ElapsedNs = LmqTimeElapsedNsSince(StartNs);

    Throughput = ((double)CACHE_SLOTS / ElapsedNs * 1'000'000'000.0);

    printf("Elapsed: %s [ms] @ %s retrieval/sec (1KB items, timing includes scheduling)\n",
            std::format("{:.0Lf}", ElapsedNs / 1'000'000.0).c_str(),
            std::format("{:.0Lf}", Throughput).c_str());

    //
    // Cleanup.
    //

    std::ranges::for_each(Threads, [](HANDLE& Thread) noexcept
    {
        CloseHandle(Thread);
    });

    CloseHandle(StartEvent);

    //
    // Clear the cache.
    //

    printf("\nClearing the cache.\n");

    StartNs = LmqGetTickCountNs();

    CHECK(LmqClearSegmentedCache(Cache,
                                 FALSE));

    ElapsedNs = LmqTimeElapsedNsSince(StartNs);

    Throughput = ((double)CACHE_SLOTS / ElapsedNs * 1'000'000'000.0);

    printf("Elapsed: %s [ms] @ %s entries/sec (frees all items)\n",
            std::format("{:.0Lf}", ElapsedNs / 1'000'000.0).c_str(),
            std::format("{:.0Lf}", Throughput).c_str());

    //
    // Done, destroy segmented cache.
    //

    CHECK(LmqDestroySegmentedCache(&Cache));

    return 0;
}
