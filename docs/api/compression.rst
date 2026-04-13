********************
LwMQ Compression API
********************

LwMQ includes fast LZ4-based functions for compressing and decompressing data buffers, which can be used in conjunction with the messaging features, or independently. The implementation uses fast processor-specific routines for optimal performance on modern hardware and should match or outperform most compression libraries as far as compression and decompression speed.

The LZ4 compression scheme offers fast compression and ultrafast decompression with reasonable compression ratio for most compressible payloads.

The API offers a one-shot BLOB mode that compresses a buffer into a blob that is allocated by the API, making usage straightforward, as well as a buffer-based compression API.

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

    LMQ_COMPRESSIONWORKSPACE
    LMQ_COMPRESSEDDATABLOB

Buffer Compression Functions
----------------------------

.. code:: cpp

    LMQAPI
    LmqAllocateCompressionWorkspace (
        SIZE_T MaxUncompressedBufferSize,
        PLMQ_COMPRESSIONWORKSPACE CompressionWorkspace
        );

    LMQAPI
    LmqFreeCompressionWorkspace (
        LMQ_COMPRESSIONWORKSPACE CompressionWorkspace
        );

    LMQAPI
    LmqCompressBuffer (
        const void* UncompressedBuffer,
        SIZE_T UncompressedBufferSize,
        void* CompressedBuffer,
        SIZE_T CompressedBufferSize,
        PSIZE_T FinalCompressedSize,
        LMQ_COMPRESSIONWORKSPACE CompressionWorkspace
        );

    LMQAPI
    LmqDecompressBuffer (
        void* UncompressedBuffer,
        SIZE_T UncompressedBufferSize,
        const void* CompressedBuffer,
        SIZE_T CompressedBufferSize,
        PSIZE_T FinalUncompressedSize
        );

One-Shot Compression Functions (BLOB)
-------------------------------------

.. code:: cpp

    LMQAPI
    LmqCompressData (
        const void* UncompressedBuffer,
        SIZE_T UncompressedBufferSize,
        PLMQ_COMPRESSEDDATABLOB CompressedDataBlob,
        PSIZE_T CompressedDataBlobSize
        );

    LMQAPI
    LmqDecompressData (
        const LMQ_COMPRESSEDDATABLOB CompressedDataBlob,
        void* UncompressedBuffer,
        PSIZE_T UncompressedBufferSize
        );

    LMQAPI
    LmqGetCompressedDataBlobSizes (
        const LMQ_COMPRESSEDDATABLOB CompressedDataBlob,
        PSIZE_T CompressedBlobSize,
        PSIZE_T UncompressedDataSize
        );

    LMQAPI
    LmqFreeCompressedDataBlob (
        PLMQ_COMPRESSEDDATABLOB CompressedDataBlob
        );

    const
    LMQ_COMPRESSEDDATABLOB
    LmqCompressedDataBlobFromPointer (
        const BYTE* Ptr
        );
