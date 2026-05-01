****************
LwMQ Storage API
****************

LwMQ provides a key-value storage API that allows applications to create and manage on-disk key-value stores for various purposes such as caching, storing application state, and more.

The storage API also includes fast LZ4-based functions for compressing and decompressing data buffers, which can be used in conjunction with the key-value storage or independently.

The LwMQ Storage component is based, in part, on a modified version of LMDB, customized for use as part is the LwMQ Storage API.

Notable modifications include support for NTFS sparse files and alternate data streams.

LMDB is licensed under the `OpenLDAP Public License`_.

.. _OpenLDAP Public License: https://github.com/LMDB/lmdb/blob/mdb.master/libraries/liblmdb/LICENSE

C and C++ Header File
=====================

.. code:: cpp

    #include <api-lwmq-storage.h>

Binaries
========

.. code:: cpp

    api-lwmq-storage-1-0.dll

Dependencies
============

.. code:: cpp

    api-lwmq-hash-1-0.dll

Overview
========

Types
-----

.. code:: cpp

    LMQ_KVSTORE
 
Key-Value Storage Functions
---------------------------

Core KV-Store Functions
^^^^^^^^^^^^^^^^^^^^^^^

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
        PCLMQ_KEY Key,
        PVOID Data,
        SIZE_T DataSize,
        LMQ_COMPRESSIONTYPE CompressionType
        );

    LMQAPI
    LmqLookupKvStoreEntry (
        LMQ_KVSTORE KvStore,
        PCLMQ_KEY Key,
        PSIZE_T DataSize
    );

    LMQAPI
    LmqRetrieveKvStoreEntry (
        LMQ_KVSTORE KvStore,
        PCLMQ_KEY Key,
        PVOID Data,
        PSIZE_T DataSize
        );

    LMQAPI
    LmqDeleteKvStoreEntry (
        LMQ_KVSTORE KvStore,
        PCLMQ_KEY Key
        );

    LMQAPI
    LmqCloseKvStore (
        PLMQ_KVSTORE KvStore
        );

KV-Store Key Helpers Functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    LMQAPI
    LmqKvStoreKeyFromStringA (
        PCSTR String,
        SIZE_T MaxLength,
        PLMQ_KEY Key
        );

    LMQAPI
    LmqKvStoreKeyFromStringW (
        PCWSTR String,
        SIZE_T MaxLength,
        PLMQ_KEY Key
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
        PLMQ_KEY Key
        );

    LMQAPI
    LmqMakeRfc4122KvStoreKey (
        PLMQ_KEY Key
        );
