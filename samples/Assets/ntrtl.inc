/*++

Copyright (c) Axel Rietschin Software Development, LLC

Module Name:

    ntrtl.inc

Abstract:

    Import NT RTL functions.

Author:

    Axel Rietschin (13-May-2026)

Environment:

    User Mode.

--*/

#pragma comment(lib, "ntdll.lib")

extern "C"
{
    NTSTATUS
    NTAPI
    RtlGetCompressionWorkSpaceSize (
        _In_ USHORT CompressionFormatAndEngine,
        _Out_ PULONG CompressBufferWorkSpaceSize,
        _Out_ PULONG CompressFragmentWorkSpaceSize
        );

    NTSTATUS
    NTAPI
    RtlCompressBuffer (
        _In_ USHORT CompressionFormatAndEngine,
        _In_reads_bytes_(UncompressedBufferSize) PUCHAR UncompressedBuffer,
        _In_ ULONG UncompressedBufferSize,
        _Out_writes_bytes_to_(CompressedBufferSize, *FinalCompressedSize) PUCHAR CompressedBuffer,
        _In_ ULONG CompressedBufferSize,
        _In_ ULONG UncompressedChunkSize,
        _Out_ PULONG FinalCompressedSize,
        _In_ PVOID WorkSpace
        );

    NTSTATUS
    NTAPI
    RtlDecompressBufferEx (
        _In_ USHORT CompressionFormat,
        _Out_writes_bytes_to_(UncompressedBufferSize, *FinalUncompressedSize) PUCHAR UncompressedBuffer,
        _In_ ULONG UncompressedBufferSize,
        _In_reads_bytes_(CompressedBufferSize) PUCHAR CompressedBuffer,
        _In_ ULONG CompressedBufferSize,
        _Out_ PULONG FinalUncompressedSize,
        _In_opt_ PVOID WorkSpace
        );
}
