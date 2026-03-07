************************
LwMQ Segmented Cache API
************************

Beyond the LwMQ in-memory cache, LwMQ provides a segmented (partitionned) cache built on top of the regular in-memory cache.

The segmented cache partitions the key space and assign each key segment to its own underlying LRU cache. The result is a composite cache that reduces contentions to enable higher insertions and lookup rate than possible with a single cache with a central LRU list.

The drawback is the cache assumes a uniform distribution of keys. The API provides for indicating which bits in the key are used for partitioning, giving the user some control.

The segmented cache supports up to 1,024 segments, and each segment can hold millions of entries. By creating as many segments as available CPU cores, up to 1,024, contentions are minimized and the cache througput can reach hundreds of millions of lookups per second, only subject to memory bandwidth, available CPU cores, and cache entry sizes.

The segmented cache supports terabytes of RAM and the number of entries it holdscan be extended at runtime. The segmented cache is suitable for highly concurrent scenarios where many threads concurrently add and retrieve cache entries, and where the key distribution is reasonably uniform or can be made uniform through the choice of partitioning bits in the key. In such scenarios, LwMQ'a segmented cache is many times faster than existing popular "O(1)" in-memory LRU caches, easily surpassing 100 million operations/sec on common server hardware with full LRU and TTL operations.

The cache also supports an elaborate "read through" mechanism that allows applications to provide a callback for cache misses, which is invoked by the cache when a cache miss occurs. The callback can then compute the value for the missing key and add it to the cache, allowing the original cache lookup to succeed without the caller having to implement its own retry logic. This mechanism is designed to be efficient and to minimize latency for cache misses, making it suitable for scenarios where cache misses are expected but should be handled gracefully without significant performance degradation. The cache also ensures correctness in concurrent scenarios where exactly one thread reads through while any other thread concurrently waiting for the same key blocks until it becomes available.

C and C++ Header File
=====================

.. code:: cpp

    #include <api-lwmq-segmented-cache.h>

Binaries
========

.. code:: cpp

    api-lwmq-cache-1-0.dll

Dependencies
============

.. code:: cpp

    api-lwmq-heap-1-0.dll
    api-lwmq-hash-1-0.dll

Overview
========

Types
-----

.. code:: cpp

    LMQ_SEGMENTEDCACHE
    LMQ_SEGMENTEDCACHE_METRICS
    LMQ_SEGMENTEDCACHE_KEYTYPE

Segmented Cache Functions
-------------------------

Core Segmented Cache Functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    LMQAPI
    LmqCreateSegmentedCache (
        PCLMQ_CACHEPARAMETERS Parameters,
        LMQ_SEGMENTEDCACHE_KEYTYPE KeyType,
        BYTE KeyWordOffset,
        WORD Segments,
        PLMQ_SEGMENTEDCACHE Cache
        );

    LMQAPI
    LmqClearSegmentedCache (
        LMQ_SEGMENTEDCACHE Cache,
        BOOL RemoveExtents
        );

    LMQAPI
    LmqDestroySegmentedCache (
        PLMQ_SEGMENTEDCACHE Cache
        );

    LMQAPI
    LmqGetSegmentedCacheMetrics (
        LMQ_SEGMENTEDCACHE Cache,
        PLMQ_SEGMENTEDCACHE_METRICS Metrics
        );

    LMQAPI
    LmqAddSegmentedCacheEntry (
        LMQ_SEGMENTEDCACHE Cache,
        PCLMQ_CACHEKEY Key,
        PVOID Data,
        ULONG DataSize,
        PCLMQ_CACHEENTROPY AdditionalEntropy,
        WORD EntryFlags,
        FLOAT TTLSec
        );

    LMQAPI
    LmqAddSegmentedCacheEntryFromCallback (
        LMQ_CACHE Cache,
        LMQ_CACHECOOKIE Cookie,
        PVOID Data,
        ULONG DataSize,
        PCLMQ_CACHEENTROPY AdditionalEntropy,
        WORD EntryFlags,
        FLOAT TTLSec
        );

    LMQAPI
    LmqRemoveSegmentedCacheEntry (
        LMQ_SEGMENTEDCACHE Cache,
        PCLMQ_CACHEKEY Key
        );

    LMQAPI
    LmqLookupSegmentedCacheEntry (
        LMQ_SEGMENTEDCACHE Cache,
        PCLMQ_CACHEKEY Key,
        PULONG DataSize
        );

    LMQAPI
    LmqRetrieveSegmentedCacheEntry (
        LMQ_SEGMENTEDCACHE Cache,
        PCLMQ_CACHEKEY Key,
        PVOID Data,
        PULONG DataSize,
        PCLMQ_CACHEENTROPY AdditionalEntropy,
        PVOID CacheMissContext
        );

    LMQAPI
    LmqRetrieveSegmentedCacheEntryFromCallback (
        LMQ_CACHE Cache,
        LMQ_CACHECOOKIE Cookie,
        PVOID Data,
        ULONG DataSize,
        PCLMQ_CACHEENTROPY AdditionalEntropy
        );

Segmented Cache Memory Allocation (Advanced)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    LMQAPI
    LmqAdvSegmentedCacheMemAlloc (
        LMQ_SEGMENTEDCACHE Cache,
        PVOID* DataPointerAddress,
        ULONG DataSize
        );

    LMQAPI
    LmqAdvSegmentedCacheMemFree (
        LMQ_SEGMENTEDCACHE Cache,
        PVOID* DataPointerAddress
        );

Cache Extents Functions (Advanced)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    LMQAPI
    LmqAdvSegmentedCacheAddExtent (
        LMQ_SEGMENTEDCACHE Cache,
        ULONG NewExtentSizeEntries
        );
