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

int main()
{
    std::locale::global(std::locale("en_US.UTF-8"));

    printf("CompressionBenchmark 1.0 - LwMQ Compression API Demo.\n\nCompressible payload text: %s bytes (%s KB). All times in microseconds.\n",
           std::format("{:L}", sizeof(PayloadText)).c_str(),
           std::format("{:.2Lf}", sizeof(PayloadText) / 1024.0).c_str());

    UINT64 StartNs{};
    UINT64 ElapsedNs{};

    //
    // One-shot compression (LZ4) ---------------------------------------------
    //

    printf("\nDemo 1: One-shot compression (LwMQ LZ4)\n");

    SIZE_T FinalCompressedSize{};
    LMQ_COMPRESSEDDATA CompressedBlob{};

    //
    // Warmup run.
    //

    CHECK(LmqCompressData(&PayloadText[0],
                          sizeof(PayloadText),
                          LMQ_COMPRESSION_LZ4,
                          &CompressedBlob,
                          &FinalCompressedSize));

    CHECK(LmqFreeCompressedData(&CompressedBlob));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqCompressData(&PayloadText[0],
                          sizeof(PayloadText),
                          LMQ_COMPRESSION_LZ4,
                          &CompressedBlob,
                          &FinalCompressedSize));

    ElapsedNs = LmqTimeElapsedNsSince(StartNs);

    CHECK(LmqFreeCompressedData(&CompressedBlob));

    printf("Elapsed: %s [us] @ %.3f GB/sec. Compressed data size: %s KB\n",
            std::format("{:.0Lf}", ElapsedNs / 1'000.0).c_str(),
            sizeof(PayloadText) / (double) ElapsedNs,
            std::format("{:.2Lf}", FinalCompressedSize / 1024.0).c_str());

    //
    // One-shot compression (Deflate) -----------------------------------------
    //

    printf("\nDemo 2: One-shot compression (LwMQ Deflate)\n");

    //
    // Warmup run.
    //

    CHECK(LmqCompressData(&PayloadText[0],
                          sizeof(PayloadText),
                          LMQ_COMPRESSION_DEFLATE,
                          &CompressedBlob,
                          &FinalCompressedSize));

    CHECK(LmqFreeCompressedData(&CompressedBlob));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqCompressData(&PayloadText[0],
                          sizeof(PayloadText),
                          LMQ_COMPRESSION_DEFLATE,
                          &CompressedBlob,
                          &FinalCompressedSize));

    ElapsedNs = LmqTimeElapsedNsSince(StartNs);

    CHECK(LmqFreeCompressedData(&CompressedBlob));

    printf("Elapsed: %s [us] @ %.3f GB/sec. Compressed data size: %s KB\n",
            std::format("{:.0Lf}", ElapsedNs / 1'000.0).c_str(),
            sizeof(PayloadText) / (double) ElapsedNs,
            std::format("{:.2Lf}", FinalCompressedSize / 1024.0).c_str());

    //
    // Buffer compression (LZ4) -----------------------------------------------
    //

    printf("\nDemo 3: Buffer compression (LwMQ LZ4)\n");

    //
    // The compressed buffer size must be larger. This is
    // counter-intuitive but for tiny payloads (bytes) and
    // some pathological ones, the compressed data is larger
    // than the plain data, and we don't want to err. 1024
    // is arbitrary. Real applications should look for
    // buffer-too-small errors and decide if they want to
    // compress that particular data or not.
    //

    const SIZE_T CompressedBufferSize{ sizeof(PayloadText) + 1024 };
    __pragma(warning(suppress: 26414)) auto CompressedBuffer = std::make_unique<BYTE[]>(CompressedBufferSize);

    if (!CompressedBuffer)
    {
        CHECK(E_OUTOFMEMORY);
    }

    //
    // Warmup run.
    //

    CHECK(LmqCompressBuffer(&PayloadText[0],
                            sizeof(PayloadText),
                            CompressedBuffer.get(),
                            CompressedBufferSize,
                            LMQ_COMPRESSION_LZ4,
                            &FinalCompressedSize,
                            nullptr));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqCompressBuffer(&PayloadText[0],
                            sizeof(PayloadText),
                            CompressedBuffer.get(),
                            CompressedBufferSize,
                            LMQ_COMPRESSION_LZ4,
                            &FinalCompressedSize,
                            nullptr));

    ElapsedNs = LmqTimeElapsedNsSince(StartNs);

    printf("Elapsed: %s [us] @ %.3f GB/sec. Compressed data size: %s KB\n",
            std::format("{:.0Lf}", ElapsedNs / 1'000.0).c_str(),
            sizeof(PayloadText) / (double) ElapsedNs,
            std::format("{:.2Lf}", FinalCompressedSize / 1024.0).c_str());

    //
    // Buffer compression (Deflate) -------------------------------------------
    //

    printf("\nDemo 4: Buffer compression (LwMQ Deflate)\n");

    //
    // Warmup run.
    //

    CHECK(LmqCompressBuffer(&PayloadText[0],
                            sizeof(PayloadText),
                            CompressedBuffer.get(),
                            CompressedBufferSize,
                            LMQ_COMPRESSION_DEFLATE,
                            &FinalCompressedSize,
                            nullptr));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqCompressBuffer(&PayloadText[0],
                            sizeof(PayloadText),
                            CompressedBuffer.get(),
                            CompressedBufferSize,
                            LMQ_COMPRESSION_DEFLATE,
                            &FinalCompressedSize,
                            nullptr));

    ElapsedNs = LmqTimeElapsedNsSince(StartNs);

    printf("Elapsed: %s [us] @ %.3f GB/sec. Compressed data size: %s KB\n",
            std::format("{:.0Lf}", ElapsedNs / 1'000.0).c_str(),
            sizeof(PayloadText) / (double) ElapsedNs,
            std::format("{:.2Lf}", FinalCompressedSize / 1024.0).c_str());

    return 0;
}
