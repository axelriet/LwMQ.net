/*++

Copyright (c) Axel Rietschin Software Development, LLC

Module Name:

    CompressionBenchmark.cpp

Abstract:

    LwMQ compression API demo/benchmark.

Prerequisites:

    The LwMQ SDK must be installed prior to building
    this sample. Get it from www.lwmq.net and make
    sure to restart VS (if it was running) to catch
    the environment variables to LwMQ's inc and lib.

Author:

    Axel Rietschin (03-May-2026)

Environment:

    User Mode.

--*/

#include <format>

#include <api-lwmq-time.h>
#include <api-lwmq-storage.h>

#include <api-lwmq-samples-common.h>

#include "..\PayloadText.inc"

void
PrintTimeAndSize (
    _In_ PCSTR Label,
    _In_ UINT64 ElapsedNs,
    _In_ SIZE_T DataSize
    )
{
    printf("%s: %s [us] @ %.3f GB/sec. Data size: %s KB\n",
           Label,
           std::format("{:.0Lf}", ElapsedNs / 1'000.0).c_str(),
           sizeof(PayloadText) / (double)ElapsedNs,
           std::format("{:.2Lf}", DataSize / 1024.0).c_str());
}

int main()
{
    std::locale::global(std::locale("en_US.UTF-8"));

    printf("CompressionBenchmark 1.0 - LwMQ Compression API Demo.\n\nCompressible payload text: %s bytes (%s KB). All times in microseconds.\n",
           std::format("{:L}", sizeof(PayloadText)).c_str(),
           std::format("{:.2Lf}", sizeof(PayloadText) / 1024.0).c_str());

    UINT64 StartNs{};

    //
    // The compressed buffer size must be larger. This is
    // counter-intuitive but for tiny payloads (bytes) and
    // some pathological ones, the compressed data is larger
    // than the plain data, and we don't want to err. 1024
    // is arbitrary. Real applications should look for
    // buffer-too-small errors and decide if they want to
    // compress that particular data or not.
    //

    constexpr SIZE_T BufferSize{ sizeof(PayloadText) + 1024 };

    __pragma(warning(suppress: 26414)) auto CompressedBuffer = std::make_unique<BYTE[]>(BufferSize);
    __pragma(warning(suppress: 26414)) auto UncompressedBuffer = std::make_unique<BYTE[]>(BufferSize);

    if (!CompressedBuffer || !UncompressedBuffer)
    {
        CHECK(E_OUTOFMEMORY);
    }

    //
    // One-shot compression (LZ4) ---------------------------------------------
    //

    printf("\nDemo 1: One-shot compression/decompression (LwMQ LZ4)\n");

    SIZE_T CompressedSize{};
    SIZE_T UncompressedSize{};

    LMQ_COMPRESSEDDATA CompressedBlob{};

    //
    // Warmup run.
    //

    CHECK(LmqCompressData(&PayloadText[0],
                          sizeof(PayloadText),
                          LMQ_COMPRESSION_LZ4,
                          &CompressedBlob,
                          &CompressedSize));

    UncompressedSize = BufferSize;

    CHECK(LmqDecompressData(CompressedBlob,
                            CompressedBuffer.get(),
                            &UncompressedSize));

    CHECK(LmqFreeCompressedData(&CompressedBlob));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqCompressData(&PayloadText[0],
                          sizeof(PayloadText),
                          LMQ_COMPRESSION_LZ4,
                          &CompressedBlob,
                          &CompressedSize));

    PrintTimeAndSize("Compression",
                     LmqTimeElapsedNsSince(StartNs),
                     CompressedSize);

    UncompressedSize = BufferSize;

    StartNs = LmqGetTickCountNs();

    CHECK(LmqDecompressData(CompressedBlob,
                            CompressedBuffer.get(),
                            &UncompressedSize));

    PrintTimeAndSize("Decompression",
                     LmqTimeElapsedNsSince(StartNs),
                     UncompressedSize);

    CHECK(LmqFreeCompressedData(&CompressedBlob));

    //
    // One-shot compression (Deflate) -----------------------------------------
    //

    printf("\nDemo 2: One-shot compression/decompression (LwMQ Deflate)\n");

    //
    // Warmup run.
    //

    CHECK(LmqCompressData(&PayloadText[0],
                          sizeof(PayloadText),
                          LMQ_COMPRESSION_DEFLATE,
                          &CompressedBlob,
                          &CompressedSize));

    UncompressedSize = BufferSize;

    CHECK(LmqDecompressData(CompressedBlob,
                            CompressedBuffer.get(),
                            &UncompressedSize));

    CHECK(LmqFreeCompressedData(&CompressedBlob));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqCompressData(&PayloadText[0],
                          sizeof(PayloadText),
                          LMQ_COMPRESSION_DEFLATE,
                          &CompressedBlob,
                          &CompressedSize));

    PrintTimeAndSize("Compression",
                     LmqTimeElapsedNsSince(StartNs),
                     CompressedSize);

    UncompressedSize = BufferSize;

    StartNs = LmqGetTickCountNs();

    CHECK(LmqDecompressData(CompressedBlob,
                            CompressedBuffer.get(),
                            &UncompressedSize));

    PrintTimeAndSize("Decompression",
                     LmqTimeElapsedNsSince(StartNs),
                     UncompressedSize);

    CHECK(LmqFreeCompressedData(&CompressedBlob));

    //
    // Buffer compression (LZ4) -----------------------------------------------
    //

    printf("\nDemo 3: CompressedBuffer compression/decompression (LwMQ LZ4)\n");

    //
    // Warmup run.
    //

    CHECK(LmqCompressBuffer(&PayloadText[0],
                            sizeof(PayloadText),
                            CompressedBuffer.get(),
                            BufferSize,
                            LMQ_COMPRESSION_LZ4,
                            &CompressedSize,
                            nullptr));

    CHECK(LmqDecompressBuffer(UncompressedBuffer.get(),
                              BufferSize,
                              CompressedBuffer.get(),
                              CompressedSize,
                              LMQ_COMPRESSION_LZ4,
                              &UncompressedSize));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqCompressBuffer(&PayloadText[0],
                            sizeof(PayloadText),
                            CompressedBuffer.get(),
                            BufferSize,
                            LMQ_COMPRESSION_LZ4,
                            &CompressedSize,
                            nullptr));

    PrintTimeAndSize("Compression",
                     LmqTimeElapsedNsSince(StartNs),
                     CompressedSize);

    StartNs = LmqGetTickCountNs();

    CHECK(LmqDecompressBuffer(UncompressedBuffer.get(),
                              BufferSize,
                              CompressedBuffer.get(),
                              CompressedSize,
                              LMQ_COMPRESSION_LZ4,
                              &UncompressedSize));

    PrintTimeAndSize("Decompression",
                     LmqTimeElapsedNsSince(StartNs),
                     UncompressedSize);

    //
    // Buffer compression (Deflate) -------------------------------------------
    //

    printf("\nDemo 4: CompressedBuffer compression/decompression (LwMQ Deflate)\n");

    //
    // Warmup run.
    //

    CHECK(LmqCompressBuffer(&PayloadText[0],
                            sizeof(PayloadText),
                            CompressedBuffer.get(),
                            BufferSize,
                            LMQ_COMPRESSION_DEFLATE,
                            &CompressedSize,
                            nullptr));

    CHECK(LmqDecompressBuffer(UncompressedBuffer.get(),
                              BufferSize,
                              CompressedBuffer.get(),
                              CompressedSize,
                              LMQ_COMPRESSION_DEFLATE,
                              &UncompressedSize));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqCompressBuffer(&PayloadText[0],
                            sizeof(PayloadText),
                            CompressedBuffer.get(),
                            BufferSize,
                            LMQ_COMPRESSION_DEFLATE,
                            &CompressedSize,
                            nullptr));

    PrintTimeAndSize("Compression",
                     LmqTimeElapsedNsSince(StartNs),
                     CompressedSize);

    StartNs = LmqGetTickCountNs();

    CHECK(LmqDecompressBuffer(UncompressedBuffer.get(),
                              BufferSize,
                              CompressedBuffer.get(),
                              CompressedSize,
                              LMQ_COMPRESSION_DEFLATE,
                              &UncompressedSize));

    PrintTimeAndSize("Decompression",
                     LmqTimeElapsedNsSince(StartNs),
                     UncompressedSize);

#if 0

    //
    // ------------------------------------------------------------------------
    //

    printf("\nDemo 5: Your compression/decompression\n");

    StartNs = LmqGetTickCountNs();

    //
    // Add your compression here. Store the compressed size in CompressedSize
    //

    PrintTimeAndSize("Compression",
                     LmqTimeElapsedNsSince(StartNs),
                     CompressedSize);

    StartNs = LmqGetTickCountNs();

    //
    // Add your decompression here. Store the uncompressed size in UncompressedSize
    //

    PrintTimeAndSize("Decompression",
                     LmqTimeElapsedNsSince(StartNs),
                     UncompressedSize);

#endif

    return 0;
}
