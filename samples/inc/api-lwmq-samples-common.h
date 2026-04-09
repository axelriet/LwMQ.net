/*++

Copyright (c) Axel Rietschin Software Development, LLC

Module Name:

    samples-common.h

Abstract:

    Common definitions for LwMQ's samples.

Author:

    Axel Rietschin (23-Mar-2026)

Environment:

    User Mode.

--*/

#pragma warning(disable: 26490) // Don't use reinterpret_cast.

#define PRINT_HR(__hr__)     do { printf("Something went wrong: 0x%08lx\n", __hr__); } __pragma(warning(suppress:4548)) while(0,0)
#define CHECK(__hr__)        do { const HRESULT __hrRet__{ __hr__ }; __pragma(warning(suppress:4127 26814 26498)) if (FAILED(__hrRet__)) [[unlikely]] { PRINT_HR(__hrRet__); exit(-1); }} __pragma(warning(suppress:4548)) while(0,0)
#define CHECK_RETURN(__hr__) do { const HRESULT __hrRet__{ __pragma(warning(suppress:6001)) __hr__ }; __pragma(warning(suppress: 4127 26814 26498)) if (FAILED(__hrRet__)) [[unlikely]] { PRINT_HR(__hrRet__); return (__hrRet__); }} __pragma(warning(suppress:4548)) while(0,0)

#ifdef USE_PRECISE_BUT_SLOWER_TIMESTAMPS 

#pragma comment(lib, "ntdll.lib")

extern "C"
NTSYSAPI
ULONGLONG
NTAPI
RtlGetSystemTimePrecise (
    VOID
    );

__declspec(selectany) volatile ULONGLONG __Dummy__[10];

FORCEINLINE
double
ComputeTimingOverhead (
    VOID
    ) noexcept
{
    //
    // Time the time function so we can subtract its
    // overhead from the observed latency so make more
    // accurate measurements.
    //

    Sleep(10);

    const ULONGLONG Start{ RtlGetSystemTimePrecise() };

    //
    // Call the function 1 million times and assign
    // the result to a variable, volatile to make sure
    // the compiler doesn't outsmart us.
    //

    for (int x = 0; x < 100'000; x++)
    {
        __Dummy__[0] = RtlGetSystemTimePrecise();
        __Dummy__[1] = RtlGetSystemTimePrecise();
        __Dummy__[2] = RtlGetSystemTimePrecise();
        __Dummy__[3] = RtlGetSystemTimePrecise();
        __Dummy__[4] = RtlGetSystemTimePrecise();
        __Dummy__[5] = RtlGetSystemTimePrecise();
        __Dummy__[6] = RtlGetSystemTimePrecise();
        __Dummy__[7] = RtlGetSystemTimePrecise();
        __Dummy__[8] = RtlGetSystemTimePrecise();
        __Dummy__[9] = RtlGetSystemTimePrecise();
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

    return Elapsed100Ns / 10'000.0;
}

#else

FORCEINLINE
ULONGLONG
LmqGetSystemTime (
    VOID
    ) noexcept
{
    return __pragma(warning(suppress:26472)) static_cast<ULONGLONG>(ReadNoFence64(reinterpret_cast<PLONG64>(0x7FFE0014)));
}

#endif

__declspec(selectany) double g_TimingAdjustmentNs{};
