****************
LwMQ Caching API
****************

LwMQ provides a caching API that allow applications to create
and manage in-memory caches for various purposes such as storing
frequently accessed data, results of expensive computations, and
more, either in raw, encrypted, compressed, or compressed and
encrypted form.

The cache scavenging strategy implemented in the LwMQ in-memory
cache is Last Recently Used (LRU) and supports features such as
time-to-live (TTL) for cache entries, encryption with additional
per-entry entropy.

The cache is passive in the sense that it does not use a background
thread for housekeeping. It is designed to be fast and efficient in
situations with low to moderate contentions, making use of modern
processor capabilities, with cache operation in the millions of
operations per second on a single thread for small payloads.

The in-memory cache supports terabytes of RAM and can be extended
at runtime. It implements a custom memory management scheme that
requests huge blocks directly from the system's virtual memory
manager for internal data structures.

It is suitable for low-contentions scenarios where only one or a
few threads concurrently add and retrieve data from the cache. For
highly concurrent scenarios, LwMQ provides a segmented cache that is
designed for high performance with many threads concurrently
accessing the cache.

The cache also supports an elaborate "read through" mechanism that
allow applications to provide a callback for cache misses, which
is invoked by the cache when a cache miss occurs.

The callback can then compute the value for the missing key and add
it to the cache, allowing the original cache lookup to succeed
without the caller having to implement its own retry logic.

This callback mechanism is designed to be efficient and to minimize
latency for cache misses, making it suitable for scenarios where
cache misses are expected but should be handled gracefully without
significant performance degradation.

The cache also ensures correctness in concurrent scenarios where
exactly one thread reads through while any other thread concurrently
waiting for the same key blocks until it becomes available.

Optionally, the segmented cache supports data compression (LZ4,
and hardware deflate through Intel QAT for very large payloads)
and AES-GCM encryption.

.. note::

    It is up to the application to supply the cache keys, which are
    in the form of an opaque 16-bytes array. The keys can be derived
    from hashing a string or byte array, or can be Globally unique
    Identifiers (GUIDs).

    LwMQ supplies functions to hash any string or byte array into
    a 128-bit key suitable for use with the cache, see
    ``LmqKeyFromString()`` and ``LmqKeyFromByteArray()``, however
    it should be observed that hash functions are not immune to
    collisions.

    The cache only stores unique keys. If the same key is added twice,
    the previous copy of the data associated with the key is discarded
    and replaced by the new data.

    This causes a theoretical risk in case of hash collision, where
    two different keys hash to the same value. The risk is that the
    second insertion overwrites the first, and querying using the
    first key will return the data from the second key.

    This risk is to be considered depending on the application. It is,
    however extremely unlikely that two similar keys ends up hashing
    to the same 128-bit value.

    To keep things in perspective, 2^128 is commensurate to the number
    of atoms in the known universe. That is a very large key space.

    Good hash functions have a property called the avalanche effect
    where most bits of the output change with a single bit change in
    the input.

    While hashing is by nature a `surjective`_ function and that there is,
    in theory, indefinitely many ways of producing the same hash when
    considering all possible inputs (all bytes ranges of any lengths that
    could possibly be conceived) it is *very* unlikely that a collision occurs
    when hashing common data types into a 128-bit hash.

    .. _surjective: https://en.wikipedia.org/wiki/Surjective_function

    Also, most people associate hashing and collisions in the context
    of a hash table, forgetting that the hash is always truncated modulo
    the number of buckets in the table, which is always a small
    number commensurate to  the number of elements in the table, times
    some (small) factor. Most collisions in hash tables come from the
    (surjective) modulo operation, not from the hash function which
    has a much larger `codomain`_.

    .. _codomain: https://en.wikipedia.org/wiki/Codomain

    Bottom line, always be cautious when using hash functions. If you
    want to alleviate any collision risk, use a GUID as key, and store
    it with your data.

    For example, say you have a session ID of some kind in your system.

    Do not hash the session ID to make a key. Instead, create a unique
    key (for example using ``LmqMakeRfc4122Key()``) and use it as
    session key. This way, no computation is needed when retrieving
    the cached session entry as the session key *is* the cache key.

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

    api-lwmq-hash-1-0.dll
    api-lwmq-heap-1-0.dll
    api-lwmq-storage-1-0.dll

Overview
========

Types
-----

.. code:: cpp

    LMQ_CACHE
    LMQ_CACHEPARAMETERS
    LMQ_CACHEMETRICS

In-Memory Cache Functions
-------------------------

Describes the thread-safe in-memory cache API.

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
        PCLMQ_KEY Key,
        PCVOID Data,
        SIZE_T DataSize,
        PCLMQ_ENTROPY AdditionalEntropy,
        WORD EntryFlags,
        FLOAT TTLSec
        );

    LMQAPI
    LmqAddCacheEntryFromCallback (
        LMQ_CACHE Cache,
        LMQ_CACHECOOKIE Cookie,
        PVOID Data,
        SIZE_T DataSize,
        PCLMQ_ENTROPY AdditionalEntropy,
        WORD EntryFlags,
        FLOAT TTLSec
        );

    LMQAPI
    LmqRemoveCacheEntry (
        LMQ_CACHE Cache,
        PCLMQ_KEY Key
        );

    LMQAPI
    LmqLookupCacheEntry (
        LMQ_CACHE Cache,
        PCLMQ_KEY Key,
        PSIZE_T DataSize
        );

    LMQAPI
    LmqRetrieveCacheEntry (
        LMQ_CACHE Cache,
        PCLMQ_KEY Key,
        PVOID Data,
        PSIZE_T DataSize,
        PCLMQ_ENTROPY AdditionalEntropy,
        PVOID CacheMissContext
        );

    LMQAPI
    LmqRetrieveCacheEntryFromCallback (
        LMQ_CACHE Cache,
        LMQ_CACHECOOKIE Cookie,
        PVOID Data,
        SIZE_T DataSize,
        PCLMQ_ENTROPY AdditionalEntropy
        );

Cache Memory Allocation (Advanced)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    LMQAPI
    LmqAdvCacheMemAlloc (
        LMQ_CACHE Cache,
        PVOID* DataPointerAddress,
        SIZE_T DataSize
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
