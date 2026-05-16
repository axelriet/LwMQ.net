/*++

Copyright (c) Axel Rietschin Software Development, LLC

Module Name:

    HashingBenchmark.cpp

Abstract:

    LwMQ Hashing, CRC, and HMAC demo/benchmark.

Prerequisites:

    The LwMQ SDK must be installed prior to building
    this sample. Get it from www.lwmq.net and make
    sure to restart VS (if it was running) to catch
    the environment variables to LwMQ's inc and lib.

Author:

    Axel Rietschin (04-May-2026)

Environment:

    User Mode.

--*/

#include <format>

#include <api-lwmq-time.h>
#include <api-lwmq-hash.h>

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

    printf("HashingBenchmark 1.0 - LwMQ Hashing, CRC, and HMAC API Demo.\n\nPayload text: %s bytes (%s KB). All times in microseconds.\n\n",
           std::format("{:L}", sizeof(PayloadText)).c_str(),
           std::format("{:.2Lf}", sizeof(PayloadText) / 1024.0).c_str());

    UINT64 StartNs{};

    //
    // CRC-16 Demo ----------------------------------------------------------------
    //
    
    UINT16 Crc16{};

    //
    // Warmup run.
    //

    CHECK(LmqBytesToCrc16(&PayloadText[0],
                          sizeof(PayloadText),
                          &Crc16));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqBytesToCrc16(&PayloadText[0],
                          sizeof(PayloadText),
                          &Crc16));

    PrintTime("CRC-16  ",
              LmqTimeElapsedNsSince(StartNs));

    //
    // CRC-32 Demo ----------------------------------------------------------------
    //
    
    UINT32 Crc32{};

    //
    // Warmup run.
    //

    CHECK(LmqBytesToCrc32(&PayloadText[0],
                          sizeof(PayloadText),
                          &Crc32));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqBytesToCrc32(&PayloadText[0],
                          sizeof(PayloadText),
                          &Crc32));

    PrintTime("CRC-32  ",
              LmqTimeElapsedNsSince(StartNs));

    //
    // CRC-64 Demo ----------------------------------------------------------------
    //
    
    UINT64 Crc64{};

    //
    // Warmup run.
    //

    CHECK(LmqBytesToCrc64(&PayloadText[0],
                          sizeof(PayloadText),
                          &Crc64));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqBytesToCrc64(&PayloadText[0],
                          sizeof(PayloadText),
                          &Crc64));

    PrintTime("CRC-64  ",
              LmqTimeElapsedNsSince(StartNs));

    //
    // Hash-64 Demo ----------------------------------------------------------------
    //
    
    UINT64 Hash64{};

    //
    // Warmup run.
    //

    CHECK(LmqBytesToHash64(&PayloadText[0],
                           sizeof(PayloadText),
                           &Hash64));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqBytesToHash64(&PayloadText[0],
                           sizeof(PayloadText),
                           &Hash64));

    PrintTime("Hash-64 ",
              LmqTimeElapsedNsSince(StartNs));
    
  
    //
    // Hash-128 Demo ----------------------------------------------------------------
    //
    
    LMQ_HASH Hash128{};

    //
    // Warmup run.
    //

    CHECK(LmqBytesToHash(&PayloadText[0],
                         sizeof(PayloadText),
                         &Hash128));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqBytesToHash(&PayloadText[0],
                         sizeof(PayloadText),
                         &Hash128));

    PrintTime("Hash-128",
              LmqTimeElapsedNsSince(StartNs));

    //
    // HMAC Demo ----------------------------------------------------------------
    //
    
    LMQ_KEY Key{};
    LMQ_HMAC HMAC{};

    CHECK(LmqMakeRfc4122Key(&Key));

    //
    // Warmup run.
    //

    CHECK(LmqComputeHMAC(&PayloadText[0],
                         sizeof(PayloadText),
                         &Key,
                         FALSE,
                         &HMAC));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqComputeHMAC(&PayloadText[0],
                         sizeof(PayloadText),
                         &Key,
                         FALSE,
                         &HMAC));

    PrintTime("HMAC    ",
              LmqTimeElapsedNsSince(StartNs));

    CHECK(LmqVerifyHMAC(&PayloadText[0],
                        sizeof(PayloadText),
                        &Key,
                        FALSE,
                        &HMAC));

    //
    // Timed run.
    //

    StartNs = LmqGetTickCountNs();

    CHECK(LmqVerifyHMAC(&PayloadText[0],
                        sizeof(PayloadText),
                        &Key,
                        FALSE,
                        &HMAC));

    PrintTime("VerifyHM",
              LmqTimeElapsedNsSince(StartNs));

    return 0;
}
