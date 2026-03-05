********************
LwMQ Compression API
********************

LwMQ includes fast LZ4-based functions for compressing and decompressing data buffers, which can be used in conjunction with the key-value storage or independently.

The compression scheme offers fast compression and ultrafast decompression with reasonable compression ratio for most compressible payloads.

The API offers a one-shot BLOB mode that compresses a buffer into a blob that is allocated by the API, making usage straightforward.

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
        INT CompressionEffort,
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
        INT CompressionEffort,
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
        LMQ_COMPRESSEDDATABLOB CompressedDataBlob
        );
    