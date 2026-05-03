/*++

Copyright (c) Axel Rietschin Software Development, LLC

Module Name:

    MiniCache.cpp

Abstract:

    LwMQ In-Memory LRU cache demo.

Prerequisites:

    The LwMQ SDK must be installed prior to building
    this sample. Get it from www.lwmq.net and make
    sure to restart VS (if it was running) to catch
    the environment variables to LwMQ's inc and lib.

Author:

    Axel Rietschin (14-Apr-2026)

Environment:

    User Mode.

--*/

#include <Windows.h>

#include <format>

#include <api-lwmq-time.h>
#include <api-lwmq-cache.h>

#include <api-lwmq-samples-common.h>

#define TINY_PAYLOAD     // Forces small payload

#define CACHE_SLOTS  (1'024 * 1'024)

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

int main()
{
    std::locale::global(std::locale("en_US.UTF-8"));

    printf("MiniCache 1.0 - LwMQ Cache Demo\nSingle Cache, %s-byte entries.\n%s slots/insertions/retrievals.\n",
           std::format("{:L}", sizeof(PayloadText)).c_str(),
           std::format("{:L}", CACHE_SLOTS).c_str());

    LMQ_KEY Key{};
    LMQ_CACHE Cache{};

    //
    // Create a key that we'll modify later.
    //

    CHECK(LmqMakeRfc4122Key(&Key));

    //
    // Create a standard LRU data cache with 1 million slots.
    //

    constexpr LMQ_CACHEPARAMETERS Parameters
    {
        sizeof(LMQ_CACHEPARAMETERS),
        LMQ_CACHETYPE_DATA_CACHE,
        LMQ_CACHEFLAGS_NONE,
        CACHE_SLOTS
    };

    CHECK(LmqCreateCache(&Parameters,
                         &Cache));

    //
    // Add 1 million entries. The key is made unique for
    // each entry simply by incrementing some part of it.
    //

    printf("\nInserting %s x 1KB entries in the LRU cache.\n",
           std::format("{:L}", CACHE_SLOTS).c_str());

    UINT64 StartNs = LmqGetTickCountNs();

    for (DWORD Index = 0; Index < CACHE_SLOTS; Index++)
    {
        //
        // We stuff the Index in the key. That makes 1 million
        // unique key although they only differ slightly.
        //

        Key.DWord0 = Index;

        CHECK(LmqAddCacheEntry(Cache,
                               &Key,
                               &PayloadText[0],
                               sizeof(PayloadText),
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
    // Retrieve all entries.
    //

    printf("Retrieving %s x 1KB entries maintaining LRU property.\n",
           std::format("{:L}", CACHE_SLOTS).c_str());

    StartNs = LmqGetTickCountNs();

    for (DWORD Index = 0; Index < CACHE_SLOTS; Index++)
    {
        Key.DWord0 = Index;

        SIZE_T DataSize{ sizeof(PayloadText) };
        BYTE RetrievedText[sizeof(PayloadText)];

        CHECK(LmqRetrieveCacheEntry(Cache,
                                    &Key,
                                    &RetrievedText[0],
                                    &DataSize,
                                    LMQ_CACHEENTRY_NO_ADDITIONAL_ENTROPY,
                                    nullptr));
    }

    ElapsedNs = LmqTimeElapsedNsSince(StartNs);

    Throughput = ((double)CACHE_SLOTS / ElapsedNs * 1'000'000'000.0);

    printf("Elapsed: %s [ms] @ %s retrieval/sec (%s-byte items @ %.1f GB/sec)\n",
            std::format("{:.0Lf}", ElapsedNs / 1'000'000.0).c_str(),
            std::format("{:.0Lf}", Throughput).c_str(),
            std::format("{:L}", sizeof(PayloadText)).c_str(),
            (Throughput * sizeof(PayloadText)) / (1024.0 * 1024.0 * 1024.0));

    //
    // Done, destroy cache.
    //

    CHECK(LmqDestroyCache(&Cache));

    return 0;
}
