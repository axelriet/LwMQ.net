**************************
LwMQ Storage API Reference
**************************

LwMQ provides a key-value storage API that allows applications to create and manage on-disk key-value stores for various purposes such as caching, storing application state, and more.

The storage API also includes fast functions for compressing and decompressing data buffers, which can be used in conjunction with the key-value storage or independently.

C and C++ Header File
=====================

.. code:: cpp

    #include <api-lwmq-storage.h>

Binaries
========

.. code:: cpp

    api-lwmq-storage.dll

Dependencies
============

.. code:: cpp

    api-lwmq-hash-1-0.dll

Types
=====

.. code:: cpp

    LMQ_KVSTORE
    LMQ_KVSTOREKEY
 
Key-Value Storage Functions
===========================

Core KV-Store Functions
-----------------------

.. code:: cpp

    LMQAPI
    LmqOpenKvStore (
        PCWSTR Pathname,
        UINT32 CreationFlags,
        SIZE_T MaximumSizeBytes,
        BOOL ReadOnly,
        PLMQ_KVSTORE KvStore
        );

    LMQAPI
    LmqAddKvStoreEntry (
        LMQ_KVSTORE KvStore,
        PCLMQ_KVSTOREKEY Key,
        PVOID Data,
        ULONG DataSize,
        INT CompressionEffort
        );

    LMQAPI
    LmqTestKvStoreEntryExists (
        LMQ_KVSTORE KvStore,
        PCLMQ_KVSTOREKEY Key
        );

    LMQAPI
    LmqRetrieveKvStoreEntry (
        LMQ_KVSTORE KvStore,
        PCLMQ_KVSTOREKEY Key,
        PVOID Data,
        PULONG DataSize
        );

    LMQAPI
    LmqDeleteKvStoreEntry (
        LMQ_KVSTORE KvStore,
        PCLMQ_KVSTOREKEY Key
        );

    LMQAPI
    LmqCloseKvStore (
        LMQ_KVSTORE KvStore
        );

KV-Store Key Helpers Functions
------------------------------

.. code:: cpp

    LMQAPI
    LmqKvStoreKeyFromStringA (
        PCSTR String,
        SIZE_T MaxLength,
        PLMQ_KVSTOREKEY Key
        );

    LMQAPI
    LmqKvStoreKeyFromStringW (
        PCWSTR String,
        SIZE_T MaxLength,
        PLMQ_KVSTOREKEY Key
        );

    #ifdef UNICODE
    #define LmqKvStoreKeyFromString  LmqKvStoreKeyFromStringW
    #else
    #define LmqKvStoreKeyFromString  LmqKvStoreKeyFromStringA
    #endif

    LMQAPI
    LmqKvStoreKeyFromByteArray (
        const BYTE* Buffer,
        SIZE_T LengthBytes,
        PLMQ_KVSTOREKEY Key
        );

    LMQAPI
    LmqMakeRfc4122KvStoreKey (
        PLMQ_KVSTOREKEY Key
        );
