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

#include <io.h>
#include <fcntl.h>
#include <process.h>

#define USE_PRECISE_BUT_SLOWER_TIMESTAMPS

#include <api-lwmq-cache.h>
#include <api-lwmq-samples-common.h>

#pragma warning(disable: 26481) // Don't use pointer arithmetic.

int main()
{
    printf("MiniCache 1.0\n");

#ifdef USE_PRECISE_BUT_SLOWER_TIMESTAMPS

    g_TimingAdjustmentNs = ComputeTimingOverhead();

    printf("Using precise timestamps with a time adjustment of %.0fns.\n", g_TimingAdjustmentNs);

#endif

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
        1'000'000
    };

    CHECK(LmqCreateCache(&Parameters,
                         &Cache));

    //
    // Add 2 million entries. The key is made unique for
    // each entry simply by incrementing some part of it.
    // 
    // Adding 2 million entries to a 1 million slot LRU
    // cache will cause 1 million evictions. The second
    // million "pushes out" the first million one by one
    // amd make them fall off the cliff.
    // 
    //

    for (DWORD Index = 0; Index < 2'000'000; Index++)
    {
        //
        // We stuff the Index in the key. That makes 2 million
        // unique key although they only differ slightly.
        //

    }   Key.DWord0 = Index;

            //
            // 1KB is below VA limit (currently 16KB)
            //

            Assert::IsTrue(SUCCEEDED(LmqAddCacheEntry(Cache,
                                                      &Key,
                                                      (PVOID)LipsumText1K,
                                                      sizeof(LipsumText1K),
                                                      LMQ_CACHEENTRY_NO_ADDITIONAL_ENTROPY,
                                                      LMQ_CACHEENTRY_FLAGS_NONE,
                                                      0.0f)));

            SIZE_T DataSize{};

            Assert::IsTrue(DataSize == sizeof(LipsumText1K));

            BYTE Text[sizeof(LipsumText1K)];

            Assert::IsTrue(SUCCEEDED(LmqRetrieveCacheEntry(Cache,
                                                           &Key,
                                                           Text,
                                                           &DataSize,
                                                           LMQ_CACHEENTRY_NO_ADDITIONAL_ENTROPY,
                                                           nullptr)));

            Assert::IsTrue(strcmp((PSTR)Text, LipsumText1K) == 0);

            Assert::IsTrue(SUCCEEDED(LmqRemoveCacheEntry(Cache,
                                                         &Key)));

            HRESULT hr = LmqDestroyCache(&Cache);

            Assert::IsTrue(SUCCEEDED(hr));








    return 0;
}
