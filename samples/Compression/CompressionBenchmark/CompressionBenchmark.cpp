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

    printf("CompressionBenchmark 1.0 - LwMQ Compression API Demo.\n\nCompressible payload text: %s bytes. All times in microseconds.\n",
           std::format("{:L}", sizeof(PayloadText)).c_str());

    UINT64 StartNs{};
    UINT64 ElapsedNs{};

    //
    // One-shot compression (LZ4)
    //

    printf("Demo 1: One-shot compression (LwMQ LZ4)\n");

    SIZE_T CompressedBlobSize{};
    LMQ_COMPRESSEDDATA CompressedBlob{};

    StartNs = LmqGetTickCountNs();

    CHECK(LmqCompressData(&PayloadText[0],
                          sizeof(PayloadText),
                          LMQ_COMPRESSION_LZ4,
                          &CompressedBlob,
                          &CompressedBlobSize));

    ElapsedNs = LmqTimeElapsedNsSince(StartNs);

    CHECK(LmqFreeCompressedData(&CompressedBlob));

    printf("Elapsed: %s [us] @ %.3f GB/sec\n",
            std::format("{:.0Lf}", ElapsedNs / 1'000.0).c_str(),
            sizeof(PayloadText) / (double) ElapsedNs);

    //
    // One-shot compression (Deflate)
    //

    printf("Demo 2: One-shot compression (LwMQ Deflate)\n");

    StartNs = LmqGetTickCountNs();

    CHECK(LmqCompressData(&PayloadText[0],
                          sizeof(PayloadText),
                          LMQ_COMPRESSION_DEFLATE,
                          &CompressedBlob,
                          &CompressedBlobSize));

    ElapsedNs = LmqTimeElapsedNsSince(StartNs);

    CHECK(LmqFreeCompressedData(&CompressedBlob));

    printf("Elapsed: %s [us] @ %.3f GB/sec\n",
            std::format("{:.0Lf}", ElapsedNs / 1'000.0).c_str(),
            sizeof(PayloadText) / (double) ElapsedNs);

    return 0;
}
