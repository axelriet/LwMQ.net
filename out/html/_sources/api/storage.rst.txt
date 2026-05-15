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

Fast Base64 Conversion Functions
--------------------------------

Quickly convert byte extents of any length to Base64 and back.

.. important::

    The Base64 representations are NOT zero-terminated.
    
    If you need a C-string, allocate one more byte and add a
    null terminator manually after the function returns the exact
    encoded size.

    Any added null terminator must NOT be counted in the encoded size
    passed to the decoder.

.. code:: cpp

    LMQAPI
    LmqBase64Encode (
        PCVOID Bytes,
        const SIZE_T Count,
        PVOID Encoded,
        PSIZE_T EncodedSize
        );

    LMQAPI
    LmqBase64Decode (
        PCVOID Encoded,
        const SIZE_T EncodedSize,
        PVOID Bytes,
        PSIZE_T Count
        );

    //
    // Helper macros for buffer size computations
    //

    #define LmqBase64EncodedBufferSizeFromInputSize(__InputSize__)

    #define LmqBase64MaxDecodedBufferSizeFromEncodedSize(__EncodedSize__)

