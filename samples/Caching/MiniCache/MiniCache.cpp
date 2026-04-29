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

    return 0;
}
