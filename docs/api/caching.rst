**************************
LWMQ Caching API Reference
**************************

LwMQ provides a caching API that allows applications to create and manage in-memory caches for various purposes such as storing frequently accessed data, results of expensive computations, and more.

The cache scavenging strategy is Last Recently Used (LRU) and supports features such as time-to-live (TTL) for cache entries, encryption with additional entropy, compression, and more.

It is designed to be fast and efficient, making use of modern processor capabilities.

C and C++ Header File
=====================

.. code:: cpp

    #include <api-lwmq-cache.h>

Binaries
========

.. code:: cpp

    api-lwmq-cache.dll

Dependencies
============

.. code:: cpp

    api-lwmq-heap-1-0.dll
    api-lwmq-hash-1-0.dll

Types
=====

.. code:: cpp

    LMQ_CACHE, PLMQ_CACHE, PCLMQ_CACHE
    LMQ_CACHEKEY, PLMQ_CACHEKEY, PCLMQ_CACHEKEY
    LMQ_CACHEENTROPY, PLMQ_CACHEENTROPY, PCLMQ_CACHEENTROPY
    LMQ_CACHEPARAMETERS, PLMQ_CACHEPARAMETERS, PCLMQ_CACHEPARAMETERS
    LMQ_CACHEMETRICS, PLMQ_CACHEMETRICS, PCLMQ_CACHEMETRICS

In-Memory Cache Functions
=========================

Core Cache Functions
--------------------

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
        _Post_ptr_invalid_ LMQ_CACHE Cache
        );

    LMQAPI
    LmqGetCacheMetrics (
        LMQ_CACHE Cache,
        _Inout_ PLMQ_CACHEMETRICS Metrics
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
----------------------------------

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
        _Post_ptr_invalid_ PVOID* DataPointerAddress
        );

Cache Extents Functions (Advanced)
----------------------------------

.. code:: cpp

    LMQAPI
    LmqAdvAddCacheExtent (
        LMQ_CACHE Cache,
        _In_range_(1, ULONG_MAX) ULONG NewExtentSizeEntries
        );

Cache Keys Helper Functions
---------------------------

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
------------------------------

.. code:: cpp

    LMQAPI
    LmqMakeCacheEntropy (
        PLMQ_CACHEENTROPY Entropy
        );
