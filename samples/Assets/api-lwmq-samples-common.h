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

#pragma warning(disable: 4714)  // __forceinline not inlined
#pragma warning(disable: 26490) // Don't use reinterpret_cast.
#pragma warning(disable: 26493) // Don't use C-style casts.

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

//
// Some WIL-similar error macros to make the rest compile
//

#ifndef RETURN_IF_WIN32_BOOL_FALSE
#define UNDEF_WIL_REPLACEMENT
#define RETURN_IF_WIN32_BOOL_FALSE(win32bool) if(win32bool == FALSE) { return HRESULT_FROM_WIN32(GetLastError()); }
#endif

#pragma warning(disable: 26481) // Don't use pointer arithmetic
#pragma warning(disable: 26461) // Argv can be marked as a pointer to const
#pragma warning(disable: 26429) // Argv can be marked as not_null

#pragma warning(push)
#pragma warning(disable: 6242)  // Jump forces local unwind (we only jump on error) 

HRESULT
FORCEINLINE
CheckPrivilege (
    PCTCH Privilege,
    _Out_ PBOOL Result
    ) noexcept
{
    *Result = FALSE;

    _Uninitialized_ HANDLE Token;

    RETURN_IF_WIN32_BOOL_FALSE(OpenProcessToken(GetCurrentProcess(),
                                                TOKEN_QUERY,
                                                &Token));

    __try
    {
        _Uninitialized_ LUID PrivilegeId;

        RETURN_IF_WIN32_BOOL_FALSE(LookupPrivilegeValue(nullptr,
                                                        Privilege,
                                                        &PrivilegeId));

        PRIVILEGE_SET Privileges{ 1, PRIVILEGE_SET_ALL_NECESSARY };

        Privileges.Privilege[0].Luid = PrivilegeId;
        Privileges.Privilege[0].Attributes = SE_PRIVILEGE_ENABLED;

        RETURN_IF_WIN32_BOOL_FALSE(PrivilegeCheck(Token,
                                                  &Privileges,
                                                  Result));
    }
    __finally
    {
        CloseHandle(Token);
    }

    return S_OK;
}

HRESULT
FORCEINLINE
CanCreateGlobalNames (
    PCSTR Argv0
    ) noexcept
{
    _Uninitialized_ BOOL HasPrivilege;

    CHECK(CheckPrivilege(SE_CREATE_GLOBAL_NAME,
                         &HasPrivilege));

    if (HasPrivilege == FALSE)
    {
        printf("The process is missing %S and will fail with Access Denied (0x80004005)\n\n", SE_CREATE_GLOBAL_NAME);

        printf("Solutions:\n\n");

        const CHAR* LastBackslash = strrchr(Argv0, '\\');
        const CHAR* ProcessName { ((LastBackslash != nullptr) && (LastBackslash[1] != '\0')) ? &LastBackslash[1] : "<this process>" };

        printf("  - Run this process with: sudo run %s (Yes, Windows has sudo!)\n", ProcessName);
        printf("  - Run this process with: powershell -Command \"Start-Process %s -Verb RunAs\"\n", ProcessName);
        printf("  - Run this process under an admin account.\n");

        printf("  - Grant %S to the account or group:\n\n", SE_CREATE_GLOBAL_NAME);

        printf("      Local Security Policy ->\n");
        printf("        Security Settings ->\n");
        printf("          Local Policies ->\n");
        printf("            User Rights Assignment ->\n");
        printf("              Create global objects ->\n");
        printf("                Properties ->\n");
        printf("                  Add User or Group...\n\n");

        return E_FAIL;
    }

    return S_OK;
}

#pragma warning(pop)

#ifdef UNDEF_WIL_REPLACEMENT
#undef RETURN_IF_WIN32_BOOL_FALSE
#endif
