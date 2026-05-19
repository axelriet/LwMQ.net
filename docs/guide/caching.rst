#####################
In-Memory LRU Caching
#####################

High-performance, concurrent LRU caches for C and C++ applications.

LwMQ provides two powerful in-memory LRU caching solutions:

* **Regular Cache** — Excellent for low-to-moderate contention workloads.
* **Segmented Cache** — Designed for extreme concurrency and massive scale (hundreds of millions of operations per second on server hardware).

Both support **zero-allocation steady-state** operation, **TTL eviction**, **safe read-through** with tombstone protection, optional **compression** (LZ4 or Intel QAT), and **AES-256-GCM encryption** with per-entry entropy.

When to Use Each Cache
======================

.. list-table::
   :header-rows: 1

   * - Use Case
     - Recommended Cache
     - Why
   * - Low-to-moderate threading (≤8-16 threads)
     - Regular Cache
     - Simpler, lower overhead
   * - Heavy multithreading + high contention
     - **Segmented Cache**
     - Direct bit partitioning eliminates global lock contention
   * - Millions to hundreds of millions ops/sec
     - **Segmented Cache**
     - Scales with cores and memory bandwidth
   * - Predictable memory usage / embedded / real-time
     - Both
     - Custom huge-block allocator + inline small data
   * - AI inference, session stores, hot data layers
     - Both (with read-through)
     - Safe single backfill + tombstone protection

Key Concepts
============

Direct Bit-Based Partitioning (Segmented Cache)
----------------------------------------------------
You control partitioning by designating 1–10 bits from your 128-bit keys. This provides **direct, zero-overhead indexing** into 2–1024 independent segments. Each segment maintains its own LRU list, dramatically reducing contention under heavy concurrency.

**Requirement**: Supply well-distributed keys (e.g., RFC 4122 GUIDs or high-quality hashes). Poor distribution leads to hot segments.

Read-Through with Tombstone Protection
--------------------------------------
On a cache miss when using a callback:

* The cache inserts a **tombstone** (placeholder) protected by a per-entry lock.
* Exactly **one** thread performs the backfill.
* Other threads wait efficiently on the same key.
* The entry is protected from eviction while the backfill is in progress.

This eliminates the thundering herd problem without locking the entire cache or segment.

Memory Management
-----------------
* Allocates one or more large virtual memory block(s) upfront.
* Uses a custom fixed-size allocator inside the block(s) → **zero runtime allocations** in steady state (especially when storing small values inline).
* Supports runtime extension via additional extents.
* Capable of handling terabytes of cached data.

Security & Efficiency Features
------------------------------
* Per-entry AES-GCM encryption with additional entropy.
* LZ4 (fast), Deflate, or hardware-accelerated Deflate (Intel QAT) compression.
* TTL-based eviction (checked on access).

Quickstart Example
==================

.. code-block:: cpp

    #include <api-lwmq-cache.h>

    LMQ_KEY Key{};
    SIZE_T DataSize{};
    LMQ_CACHE Cache{};
 
    //
    // Prepare cache parameters for 1 million slots.
    //

    constexpr LMQ_CACHEPARAMETERS Parameters
    {
        sizeof(LMQ_CACHEPARAMETERS),
        LMQ_CACHETYPE_DATA_CACHE,
        LMQ_CACHEFLAGS_NONE,
        1'000'000
    };

    //
    // Create one key.
    //

    LmqMakeRfc4122Key(&Key);

    //
    // Create the cache.
    //

    LmqCreateCache(&Parameters,
                   &Cache);

    //
    // Add one entry.
    //

    LmqAddCacheEntry(Cache,
                     &Key,
                     Data,
                     Length,
                     LMQ_CACHEENTRY_NO_ADDITIONAL_ENTROPY,
                     LMQ_CACHEENTRY_FLAGS_NONE,
                     0.0f);

    //
    // Existence test: lookup one entry.
    //

    LmqLookupCacheEntry(Cache,
                        &Key,
                        &DataSize);

    //
    // Retrieve one entry.
    //

    LmqRetrieveCacheEntry(Cache,
                          &Key,
                          Data,
                          &DataSize,
                          LMQ_CACHEENTRY_NO_ADDITIONAL_ENTROPY,
                          nullptr);

    //
    // Delete an entry.
    //

    LmqRemoveCacheEntry(Cache,
                        &Key);

    //
    // Destroy the cache.
    //

    LmqDestroyCache(&Cache);

Best Practices
==============

Key Selection
-------------

* Prefer **GUIDs** created with ``LmqMakeRfc4122Key()`` — no hashing required at lookup time.
* For strings or binary data, use ``LmqStringToKey()`` or ``LmqBytesToKey()``.
* For Segmented Cache, choose partition bits from high-entropy positions in the key.

Choosing Segment Count (Segmented Cache)
----------------------------------------

* Start with **256** or **512** segments on high-core-count servers.
* More segments = better scalability, slightly higher memory overhead.
* Fewer segments = more contention.

Read-Through Callbacks
----------------------

* Keep callbacks reasonably fast.
* The cache protects the entry (via tombstone) during the callback.

Performance Tips
----------------

* Use small inline data when possible.
* Enable Hardware Lock Elision (HLE) where supported by the CPU.
* Pre-size the cache appropriately to minimize extent allocations.
* Monitor performance with ``LmqGetCacheMetrics()``.

Performance Characteristics
===========================

* **Regular Cache**: Multi-million ops/sec even under moderate contention.
* **Segmented Cache**: Tens of millions ops/sec on laptops; **hundreds of millions** on multi-socket servers with sufficient memory bandwidth.
* Near-linear scaling with cores when keys are well-distributed.
* Predictable low latency (no background threads or global locks).

Limitations & Considerations
============================

* Keys are 128-bit values. Collision risk is extremely low but present if using weak hashing.
* Segmented Cache requires well-distributed keys for optimal load balancing.
* Encryption and compression add CPU overhead (valuable trade-off for memory/IO-bound workloads).
* The cache is passive: eviction and TTL checks occur on access.

Next Steps
==========

* Full **API Reference** → :doc:`/api/caching`
* Advanced topics: memory extents, metrics, custom allocators (see API documentation)
