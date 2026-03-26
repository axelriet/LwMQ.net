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

#define PRINT_HR(__hr__)     do { printf("Something went wrong: 0x%08lx\n", __hr__); } while(0,0)
#define CHECK(__hr__)        do { const HRESULT __hrRet__{ __hr__ }; __pragma(warning(suppress:4127 26814 26498)) if (FAILED(__hrRet__)) [[unlikely]] { PRINT_HR(__hrRet__); exit(-1); }} while(0,0)
#define CHECK_RETURN(__hr__) do { const HRESULT __hrRet__{ __pragma(warning(suppress:6001)) __hr__ }; __pragma(warning(suppress: 4127 26814 26498)) if (FAILED(__hrRet__)) [[unlikely]] { PRINT_HR(__hrRet__); return (__hrRet__); }} while(0,0)

#ifdef USE_PRECISE_BUT_SLOWER_TIMESTAMPS 

#pragma comment(lib, "ntdll.lib")

extern "C"
NTSYSAPI
ULONGLONG
NTAPI
RtlGetSystemTimePrecise (
    VOID
    );

#else

FORCEINLINE
ULONGLONG
LmqGetSystemTime (
    VOID
    ) noexcept
{
    return ReadNoFence64(reinterpret_cast<PLONG64>(0x7FFE0014));
}

#endif
