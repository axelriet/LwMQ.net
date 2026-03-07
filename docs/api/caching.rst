****************
LwMQ Caching API
****************

LwMQ provides a caching API that allows applications to create and manage in-memory caches for various purposes such as storing frequently accessed data, results of expensive computations, and more.

The cache scavenging strategy implemented in the LwMQ in-memory cache is Last Recently Used (LRU) and supports features such as time-to-live (TTL) for cache entries, encryption with additional entropy, compression, and more.

The cache is passive in the sense that it does not use a background thread for housekeeping. It is designed to be fast and efficient with low to moderate contentions, making use of modern processor capabilities, with cache operation in the millions of operations per second on a single thread.

The in-memory cache supports terabytes of RAM and can be extended at runtime. It is suitable for low-contentions scenarios where only one or a few threads concurrently add and retrieve data from the cache. For highly concurrent scenarios, LwMQ provides a segmented cache that is designed for high performance with many threads concurrently accessing the cache.

C and C++ Header File
=====================

.. code:: cpp

    #include <api-lwmq-cache.h>

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

    LMQ_CACHE
    LMQ_CACHEKEY
    LMQ_CACHEENTROPY
    LMQ_CACHEPARAMETERS
    LMQ_CACHEMETRICS

In-Memory Cache Functions
-------------------------

Core Cache Functions
^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    LMQAPI
    LmqCreateCache (
        PCLMQ_CACHEPARAMETERS Parameters,
        PLMQ_CACHE Cache
        );

    LMQAPI
    LmqClearCache (
        LMQ_CACHE Cache,
        BOOL RemoveExtents
        );

    LMQAPI
    LmqDestroyCache (
        PLMQ_CACHE Cache
        );

    LMQAPI
    LmqGetCacheMetrics (
        LMQ_CACHE Cache,
        PLMQ_CACHEMETRICS Metrics
        );

    LMQAPI
    LmqAddCacheEntry (
        LMQ_CACHE Cache,
        PCLMQ_CACHEKEY Key,
        PVOID Data,
        ULONG DataSize,
        PCLMQ_CACHEENTROPY AdditionalEntropy,
        WORD EntryFlags,
        FLOAT TTLSec
        );

    LMQAPI
    LmqAddEntryFromCacheCallback (
        LMQ_CACHE Cache,
        LMQ_CACHECOOKIE Cookie,
        PVOID Data,
        ULONG DataSize,
        PCLMQ_CACHEENTROPY AdditionalEntropy,
        WORD EntryFlags,
        FLOAT TTLSec
        );

    LMQAPI
    LmqRemoveCacheEntry (
        LMQ_CACHE Cache,
        PCLMQ_CACHEKEY Key
        );

    LMQAPI
    LmqLookupCacheEntry (
        LMQ_CACHE Cache,
        PCLMQ_CACHEKEY Key,
        PULONG DataSize
        );

    LMQAPI
    LmqRetrieveCacheEntry (
        LMQ_CACHE Cache,
        PCLMQ_CACHEKEY Key,
        PVOID Data,
        PULONG DataSize,
        PCLMQ_CACHEENTROPY AdditionalEntropy,
        PVOID CacheMissContext
        );

    LMQAPI
    LmqRetrieveCacheEntryFromCallback (
        LMQ_CACHE Cache,
        LMQ_CACHECOOKIE Cookie,
        PVOID Data,
        ULONG DataSize,
        PCLMQ_CACHEENTROPY AdditionalEntropy
        );

Cache Memory Allocation (Advanced)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    LMQAPI
    LmqAdvCacheMemAlloc (
        LMQ_CACHE Cache,
        PVOID* DataPointerAddress,
        ULONG DataSize
        );

    LMQAPI
    LmqAdvCacheMemFree (
        LMQ_CACHE Cache,
        PVOID* DataPointerAddress
        );

Cache Extents Functions (Advanced)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    LMQAPI
    LmqAdvAddCacheExtent (
        LMQ_CACHE Cache,
        ULONG NewExtentSizeEntries
        );

Cache Keys Helper Functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    LMQAPI
    LmqCacheKeyFromStringA (
        PCSTR String,
        SIZE_T MaxLength,
        PLMQ_CACHEKEY Key
        );

    LMQAPI
    LmqCacheKeyFromStringW (
        PCWSTR String,
        SIZE_T MaxLength,
        PLMQ_CACHEKEY Key
        );

    #ifdef UNICODE
    #define LmqCacheKeyFromString  LmqCacheKeyFromStringW
    #else
    #define LmqCacheKeyFromString  LmqCacheKeyFromStringA
    #endif

    LMQAPI
    LmqCacheKeyFromByteArray (
        const BYTE* Buffer,
        SIZE_T LengthBytes,
        PLMQ_CACHEKEY Key
        );

    LMQAPI
    LmqMakeRfc4122CacheKey (
        PLMQ_CACHEKEY Key
        );

Cache Entropy Helper Functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    LMQAPI
    LmqMakeCacheEntropy (
        PLMQ_CACHEENTROPY Entropy
        );
