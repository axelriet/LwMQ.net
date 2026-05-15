/*++

Copyright (c) Axel Rietschin Software Development, LLC

Module Name:

    Base64Benchmark.cpp

Abstract:

    LwMQ Base64 demo/benchmark.

Prerequisites:

    The LwMQ SDK must be installed prior to building
    this sample. Get it from www.lwmq.net and make
    sure to restart VS (if it was running) to catch
    the environment variables to LwMQ's inc and lib.

Author:

    Axel Rietschin (15-May-2026)

Environment:

    User Mode.

--*/

#include <format>

#include <api-lwmq-time.h>
#include <api-lwmq-storage.h>

#include <api-lwmq-samples-common.h>

#include <PayloadText.inc>

void
PrintTime (
    _In_ PCSTR Label,
    _In_ UINT64 ElapsedNs
    )
{
    printf("%s: %s [us] @ %.3f GB/sec.\n",
           Label,
           std::format("{:.1Lf}", ElapsedNs / 1'000.0).c_str(),
           sizeof(PayloadText) / (double)ElapsedNs);
}

int main()
{
    std::locale::global(std::locale("en_US.UTF-8"));

    printf("Base64Benchmark 1.0 - LwMQ Base64 API Demo.\n\nPayload text: %s bytes (%s KB). All times in microseconds.\n\n",
           std::format("{:L}", sizeof(PayloadText)).c_str(),
           std::format("{:.2Lf}", sizeof(PayloadText) / 1024.0).c_str());

    UINT64 StartNs{};

    //
    // CRC-16 Demo ----------------------------------------------------------------
    //
    
    SIZE_T EncodedSize{ LmqBase64EncodedBufferSizeFromInputSize(sizeof(PayloadText)) };
    auto EncodedBuffer = std::make_unique<CHAR[]>(EncodedSize);

    SIZE_T DecodedSize{ LmqBase64MaxDecodedBufferSizeFromEncodedSize(sizeof(PayloadText)) };
    auto DecodedBuffer = std::make_unique<BYTE[]>(DecodedSize);

    //
    // Warmup run.
    //

    CHECK(LmqBase64Encode(&PayloadText[0],
                          sizeof(PayloadText),
                          EncodedBuffer.get(),
                          &EncodedSize));

    CHECK(LmqBase64Decode(EncodedBuffer.get(),
                          EncodedSize,
                          DecodedBuffer.get(),
                          &DecodedSize));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqBase64Encode(&PayloadText[0],
                          sizeof(PayloadText),
                          EncodedBuffer.get(),
                          &EncodedSize));

    PrintTime("Base64Encode",
              LmqTimeElapsedNsSince(StartNs));

    StartNs = LmqGetTickCountNs();

    CHECK(LmqBase64Decode(EncodedBuffer.get(),
                          EncodedSize,
                          DecodedBuffer.get(),
                          &DecodedSize));

    PrintTime("Base64Decode",
              LmqTimeElapsedNsSince(StartNs));

    EncodedBuffer.reset();
    DecodedBuffer.reset();

    return 0;
}
