****************
LwMQ Hashing API
****************

LwMQ provides a set of hashing and HMAC functions that can be used for various purposes such as message integrity checks, caching, and more.

Remember that every hash function is subject to collisions, and the hashing functions provided by LwMQ are not cryptographic. They are designed for speed and should not be used for security-sensitive applications.

C and C++ Header File
=====================

.. code:: cpp

    #include <api-lwmq-hash.h>

Binaries
========

.. code:: cpp

    api-lwmq-hash-1-0.dll

Dependencies
============

.. code:: cpp

    None

Types
=====

.. code:: cpp

    LMQ_KEY
    LMQ_HASH
    LMQ_HMAC
    LMQ_HMAC_CONTEXT

Hashing and HMAC Functions
==========================

Hashing
-------

The hashing functions are meant to be fast. They are not cryptographic and should not be used for security purposes.

.. code:: cpp

    LMQAPI
    LmqHashByteArray32 (
        const BYTE* Buffer,
        SIZE_T LengthBytes,
        PUINT32 Hash
        );

    LMQAPI
    LmqHashByteArray64 (
        const BYTE* Buffer,
        SIZE_T LengthBytes,
        PUINT64 Hash
        );

    LMQAPI
    LmqHashByteArray128 (
        const BYTE* Buffer,
        SIZE_T LengthBytes,
        PLMQ_HASH Hash
        );

    LMQAPI_(BOOL)
    LmqCompareHashes (
        _In_ PCLMQ_HASH Hash1,
        _In_ PCLMQ_HASH Hash2
        );

    //
    // C++ operator overloads for 128-bit hashes comparisons.
    //

    bool operator==(const LMQ_HASH& Hash1, const LMQ_HASH& Hash2);

    bool operator!=(const LMQ_HASH& Hash1, const LMQ_HASH& Hash2);

HMAC (Hash-Based Message Authentication Code)
---------------------------------------------

The encryption and hashing algorithms used to compute HMACs in LwMQ are meant to be fast as their primary design goal. The cipher used is a weakened (low round count) version of an otherwise secure algorithm tuned for excellent performance on modern processors.

The intent is to provide a way for security architects to include message authentication codes in their LwMQ messaging payload with reasonabe impact on performance, knowing the data links are supposed to be relatively secure within the realms of applicability of LwMQ.

For highly secure cryptographic operations, consider using well known facilities such as `CryptoAPI`_, Crypto Next Generation (`CNG`_), or `OpenSSL`_.

.. _CryptoAPI: https://docs.microsoft.com/en-us/windows/win32/seccrypto/cryptoapi
.. _CNG: https://docs.microsoft.com/en-us/windows/win32/seccng/cng-portal
.. _OpenSSL: https://www.openssl.org/

One-shot HMAC Functions
^^^^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    LMQAPI
    LmqComputeHMAC (
        const BYTE* Buffer,
        SIZE_T LengthBytes,
        PCLMQ_KEY Key,
        PLMQ_HMAC HMAC
        );

    LMQAPI
    LmqVerifyHMAC (
        const BYTE* Buffer,
        SIZE_T LengthBytes,
        PCLMQ_KEY Key,
        PCLMQ_HMAC HMAC
        );

Streaming HMAC Functions
^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    LMQAPI
    LmqInitHMACEx (
        PCLMQ_KEY Key,
        PLMQ_HMAC_CONTEXT Context
        );

    LMQAPI
    LmqComputeHMACEx (
        const BYTE* Buffer,
        SIZE_T LengthBytes,
        PCLMQ_HMAC_CONTEXT Context,
        PLMQ_HMAC HMAC
        );

    LMQAPI
    LmqVerifyHMACEx (
        const BYTE* Buffer,
        SIZE_T LengthBytes,
        PCLMQ_HMAC_CONTEXT Context,
        PCLMQ_HMAC HMAC
        );

    LMQAPI
    LmqDestroyHMACEx (
        PLMQ_HMAC_CONTEXT Context
        );

HMAC Comparison Functions
^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: cpp

    LMQAPI_(BOOL)
    LmqCompareHMACs (
        PCLMQ_HMAC HMAC1,
        PCLMQ_HMAC HMAC2
        );

    //
    // C++ operator overloads for HMAC comparison
    //
    
    bool operator==(const LMQ_HMAC& HMAC1, const LMQ_HMAC& HMAC2)

    bool operator!=(const LMQ_HMAC& HMAC1, const LMQ_HMAC& HMAC2)
