********************
LwMQ Compression API
********************

LwMQ includes fast LZ4 and Deflate functions for compressing and
decompressing data buffers, which can be used in conjunction with
the messaging features, or independently.

The implementation uses fast processor-specific routines for
optimal performance on modern hardware and should match or
outperform most compression libraries as far as compression
and decompression speed with a reasonable compression ratio
given the chosen compression scheme.

The LZ4 compression scheme offers fast compression and ultrafast
decompression with reasonable compression ratio for most
compressible payloads, while the Deflate compression scheme is
more thorough and provides better compression at the expense of
speed, in particular decompression speed compared to LZ4.

The Deflate implementation in LwMQ still favors speed over
compression ratio. Expect the ratio to be approximately
comparable to Zlib at level 2. If your application requires
maximum compression, consider using Zlib or some other library
externally.

The API offers a one-shot BLOB mode that compresses a buffer into
a blob that is allocated by the API, making usage straightforward,
as well as the more traditional but very easy to use buffer-based
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
    LMQ_COMPRESSEDDATA

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
        PCVOID Bytes,
        SIZE_T Count,
        PVOID CompressedBuffer,
        SIZE_T CompressedBufferSize,
        LMQ_COMPRESSIONTYPE CompressionType,
        PSIZE_T FinalCompressedSize,
        LMQ_COMPRESSIONWORKSPACE CompressionWorkspace // Optional
        );

    LMQAPI
    LmqDecompressBuffer (
        PCVOID CompressedBuffer,
        SIZE_T CompressedBufferSize,
        PVOID Bytes,
        SIZE_T Count,
        LMQ_COMPRESSIONTYPE CompressionType,
        PSIZE_T FinalDecompressedSize // Optional
        );

One-Shot Compression Functions (BLOB)
-------------------------------------

.. code:: cpp

    LMQAPI
    LmqCompressData (
        PCVOID Byte,
        SIZE_T Count,
        LMQ_COMPRESSIONTYPE CompressionType,
        PLMQ_COMPRESSEDDATA CompressedDataBlob,
        PSIZE_T CompressedDataBlobSize
        );

    LMQAPI
    LmqDecompressData (
        const LMQ_COMPRESSEDDATA CompressedDataBlob,
        PVOID Byte,
        PSIZE_T Count
        );

    LMQAPI
    LmqGetCompressedDataSize (
        const LMQ_COMPRESSEDDATA CompressedDataBlob,
        PSIZE_T CompressedBlobSize,
        PSIZE_T UncompressedDataSize
        );

    LMQAPI
    LmqFreeCompressedData (
        PLMQ_COMPRESSEDDATA CompressedDataBlob
        );

    const
    LMQ_COMPRESSEDDATA
    LmqPointerToCompressedData (
        PCVOID Pointer
        );
