****************
LwMQ Hashing API
****************

LwMQ provides a set of fast hashing and HMAC functions that can be used for various purposes such as message integrity checks, and more.

Remember that every hash function is subject to collisions, and the hashing functions provided by LwMQ are not cryptographic. They are designed for speed and should not be used for security-sensitive applications. However, they provide a fast solution for everyday message integrity checks, enabling application to include checksums and HMACs at a reasonable performance cost.

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

Overview
========

Types
-----

.. code:: cpp

    LMQ_KEY
    LMQ_HASH
    LMQ_ENTROPY
    LMQ_HMAC
    LMQ_HMAC_CONTEXT

Hashing Functions
-----------------

The hashing functions are meant to be fast. They are not cryptographic and should not be used for security purposes. The hash comparison function is designed to be resistant to timing attacks by taking the same amount of time regardless of how many bytes match between the two hashes.

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
    LmqAreEqualHashes (
        PCLMQ_HASH Hash1,
        PCLMQ_HASH Hash2
        );

    //
    // C++ operator overloads for 128-bit hashes comparisons.
    //

    bool operator==(const LMQ_HASH& Hash1, const LMQ_HASH& Hash2);

    bool operator!=(const LMQ_HASH& Hash1, const LMQ_HASH& Hash2);

HMAC Functions
--------------

The encryption and hashing algorithms used to compute HMACs in LwMQ are meant to be fast as their primary design goal. The cipher used is a weakened (low round count) version of an otherwise secure algorithm tuned for excellent performance on modern processors.

The intent is to provide a way for security architects to include message authentication codes in their LwMQ messaging payload with reasonabe impact on performance, knowing the data links are supposed to be relatively secure within the realms of applicability of LwMQ.

You can protect the keys using LmqProtectKey(). In that case, you can pass the protected key to LmqComputeHMAC() and LmqVerifyHMAC() provided that you set the IsProtectedKey parameter to TRUE. The functions take care of unprotecting a copy of the key and of properly erasing the unprotected copy when no longer needed.

For highly secure cryptographic operations, consider using well known facilities such as `CryptoAPI`_, Crypto Next Generation (`CNG`_), or `OpenSSL`_.

.. _CryptoAPI: https://learn.microsoft.com/en-us/windows/win32/seccrypto/cryptoapi-system-architecture
.. _CNG: https://docs.microsoft.com/en-us/windows/win32/seccng/cng-portal
.. _OpenSSL: https://www.openssl.org/

One-shot HMAC Functions
^^^^^^^^^^^^^^^^^^^^^^^

THis set of functions is meant for cases where only a few HMACs need to be computed or verified, and the caller does not mind paying the cost of key setup every time. This simplicity comes at some preformance cost.

.. code:: cpp

    LMQAPI
    LmqComputeHMAC (
        const BYTE* Buffer,
        SIZE_T LengthBytes,
        PCLMQ_KEY Key,
        BOOL IsProtectedKey,
        PLMQ_HMAC HMAC
        );

    LMQAPI
    LmqVerifyHMAC (
        const BYTE* Buffer,
        SIZE_T LengthBytes,
        PCLMQ_KEY Key,
        BOOL IsProtectedKey,
        PCLMQ_HMAC HMAC
        );

HMAC Functions
^^^^^^^^^^^^^^

This set of functions is meant for cases where many HMACs need to be computed or verified with the same key, and the caller wants to pay the cost of key setup only once. The called typically calls LmqInitHMACEx() once then calls LmqComputeHMACEx() or LmqVerifyHMACEx() as many times as needed, then calls LmqDestroyHMACEx() to free the context when no longer needed, for example at program exit.

You can protect the keys using LmqProtectKey(). In that case, you can pass the protected key to LmqInitHMACEx() provided that you set the IsProtectedKey parameter to TRUE. The function will unprotect a copy of the key and properly erase it when no longer needed. Additionaly, the Context is encrypted when using a protected key. This helps protect against memory dumps saving the decryption context intact by reducing the time where the context is unencypted in memory. The function uses a copy of the context and decrypts it, then safely disposes of the copy when no longer needed. Note, however, that encryption/decryption of the key and context incurs a performance penalty, and that there is still a narrow window of opportunity for an attacker to dump the memory while the context is unencrypted.

If you use protected keys, it is NOT recommended to unprotect the key prior to calling LmqInitHMACEx() with the IsProtectedKey parameter set to FALSE, as in this case the context will not be encrypted. 

.. code:: cpp

    LMQAPI
    LmqInitHMACEx (
        PCLMQ_KEY Key,
        BOOL IsProtectedKey,
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

HMAC Equality Functions
^^^^^^^^^^^^^^^^^^^^^^^

This function provides a fast way to compare HMACs, which can be used for example to verify that a computed HMAC matches an expected value. The function is designed to be resistant to timing attacks by taking the same amount of time regardless of how many bytes match between the two HMACs.

.. code:: cpp

    LMQAPI_(BOOL)
    LmqAreEqualHMACs (
        PCLMQ_HMAC HMAC1,
        PCLMQ_HMAC HMAC2
        );

    //
    // C++ operator overloads for HMAC comparison
    //
    
    bool operator==(const LMQ_HMAC& HMAC1, const LMQ_HMAC& HMAC2)

    bool operator!=(const LMQ_HMAC& HMAC1, const LMQ_HMAC& HMAC2)

Keys Functions
--------------

The key functions allow you to create LMQ_KEY structures from ASCII and Unicode strings, or byte arrays, and to generate RFC 4122 Type 4 random keys.

The keys can then be used for HMAC computations or other purposes, and also be protected in memory using LmqProtectKey().

.. code:: cpp

    LMQAPI
    LmqKeyFromStringA (
        PCSTR String,
        SIZE_T MaxLength,
        PLMQ_KEY Key
        );

    LMQAPI
    LmqKeyFromStringW (
        PCWSTR String,
        SIZE_T MaxLength,
        PLMQ_KEY Key
        );

    #ifdef UNICODE
    #define LmqKeyFromString  LmqKeyFromStringW
    #else
    #define LmqKeyFromString  LmqKeyFromStringA
    #endif

    LMQAPI
    LmqKeyFromByteArray (
        const BYTE* Buffer,
        SIZE_T LengthBytes,
        PLMQ_KEY Key
        );

    LMQAPI
    LmqMakeRfc4122Key (
        PLMQ_KEY Key
        );

Entropy Functions
-----------------

This function fills the provided LMQ_ENTROPY structure with random data that can be used as a source of entropy for various purposes, such as generating random keys or nonces.

The quality of the entropy provided by this function depends on the underlying implementation and the platform's capabilities.

It is designed to be fast and suitable for general use cases, but it may not be suitable for cryptographic purposes where high-quality randomness is required.

.. code:: cpp

    LMQAPI
    LmqMakeEntropy (
        PLMQ_ENTROPY Entropy
        );

Key Protection Functions
------------------------

Those functions allow you to protect and unprotect keys in memory for use within a particular process instance's lifetime, which can help mitigate the risk of key exposure through memory dumps.

When you protect a key using LmqProtectKey(), the key is encrypted in place. You can then pass the protected key to the HMAC functions with the IsProtectedKey parameter set to TRUE, and the functions will take care of unprotecting a copy of the key and properly erasing the unprotected copy when no longer needed.

You can also unprotect a key using LmqUnprotectKey() if you need to use the unprotected key for other purposes, but be aware that this increases the risk of key exposure.

When you are done with a key, you should call LmqEraseKey() to securely erase a key, protected or not, from memory.

.. code:: cpp

    LMQAPI
    LmqProtectKey (
        PLMQ_KEY KeyToProtectInPlace
        );

    LMQAPI
    LmqUnprotectKey (
        PCLMQ_KEY ProtectedKey,
        PLMQ_KEY UnprotectedDestinationKey
        );

    LMQAPI
    LmqEraseKey (
        PLMQ_KEY Key
        );
