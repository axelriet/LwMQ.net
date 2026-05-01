********************
LwMQ Compression API
********************

LwMQ includes fast LZ4 and Deflate functions for compressing and
decompressing data buffers, which can be used in conjunction with
the messaging features, or independently.

The implementation uses fast processor-specific routines for
optimal performance on modern hardware and should match or
outperform most compression libraries as far as compression
and decompression speed.

The LZ4 compression scheme offers fast compression and ultrafast
decompression with reasonable compression ratio for most
compressible payloads, while the Deflate compression scheme is
more thorough and provides better compression at the expense of
speed.

The Deflate implementation in LwMQ still favors speed over
compression ratio. Expect the ratio to be approximately
comparable to Zlib at level 2. If your application requires
maximum compression, consider using Zlib or some other library
externally.

The API offers a one-shot BLOB mode that compresses a buffer into
a blob that is allocated by the API, making usage straightforward,
as well as a more traditional but very easy to use buffer-based
compression API.

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

    LMQ_COMPRESSIONTYPE
    LMQ_COMPRESSIONWORKSPACE
    LMQ_COMPRESSEDDATABLOB

Buffer Compression Functions
----------------------------

.. code:: cpp

    LMQAPI
    LmqAllocateCompressionWorkspace (
        LMQ_COMPRESSIONTYPE CompressionType,
        PLMQ_COMPRESSIONWORKSPACE CompressionWorkspace
        );

    LMQAPI
    LmqFreeCompressionWorkspace (
        PLMQ_COMPRESSIONWORKSPACE CompressionWorkspace
        );

    LMQAPI
    LmqCompressBuffer (
        PCVOID UncompressedBuffer,
        SIZE_T UncompressedBufferSize,
        PVOID CompressedBuffer,
        SIZE_T CompressedBufferSize,
        LMQ_COMPRESSIONTYPE CompressionType,
        PSIZE_T FinalCompressedSize,
        LMQ_COMPRESSIONWORKSPACE CompressionWorkspace // Optional
        );

    LMQAPI
    LmqDecompressBuffer (
        PVOID UncompressedBuffer,
        SIZE_T UncompressedBufferSize,
        PCVOID CompressedBuffer,
        SIZE_T CompressedBufferSize,
        LMQ_COMPRESSIONTYPE CompressionType,
        PSIZE_T FinalUncompressedSize
        );

One-Shot Compression Functions (BLOB)
-------------------------------------

.. code:: cpp

    LMQAPI
    LmqCompressData (
        PCVOID UncompressedBuffer,
        SIZE_T UncompressedBufferSize,
        LMQ_COMPRESSIONTYPE CompressionType,
        PLMQ_COMPRESSEDDATABLOB CompressedDataBlob,
        PSIZE_T CompressedDataBlobSize
        );

    LMQAPI
    LmqDecompressData (
        const LMQ_COMPRESSEDDATABLOB CompressedDataBlob,
        PVOID UncompressedBuffer,
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
