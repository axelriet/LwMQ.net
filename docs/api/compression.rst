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
        LONG MaxUncompressedBufferSize,
        PLMQ_COMPRESSIONWORKSPACE CompressionWorkspace
        );

    LMQAPI
    LmqFreeCompressionWorkspace (
        LMQ_COMPRESSIONWORKSPACE CompressionWorkspace
        );

    LMQAPI
    LmqCompressBuffer (
        UCHAR const* UncompressedBuffer,
        LONG UncompressedBufferSize,
        PUCHAR CompressedBuffer,
        LONG CompressedBufferSize,
        PLONG FinalCompressedSize,
        LMQ_COMPRESSIONWORKSPACE CompressionWorkspace
        );

    LMQAPI
    LmqDecompressBuffer (
        PUCHAR UncompressedBuffer,
        LONG UncompressedBufferSize,
        UCHAR const* CompressedBuffer,
        LONG CompressedBufferSize,
        PLONG FinalUncompressedSize
        );

One-Shot Compression Functions (BLOB)
-------------------------------------

.. code:: cpp

    LMQAPI
    LmqCompressData (
        UCHAR const* UncompressedBuffer,
        LONG UncompressedBufferSize,
        PLMQ_COMPRESSEDDATABLOB CompressedDataBlob,
        PLONG CompressedDataBlobSize
        );

    LMQAPI
    LmqDecompressData (
        LMQ_COMPRESSEDDATABLOB CompressedDataBlob,
        PUCHAR UncompressedBuffer,
        PLONG UncompressedBufferSize
        );

    LMQAPI
    LmqGetCompressedDataBlobSizes (
        LMQ_COMPRESSEDDATABLOB CompressedDataBlob,
        PLONG CompressedBlobSize,
        PLONG UncompressedDataSize
        );

    LMQAPI
    LmqFreeCompressedDataBlob (
        PLMQ_COMPRESSEDDATABLOB CompressedDataBlob
        );
    