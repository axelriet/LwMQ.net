*************************************
LwMQ Hashing, CRC, HMAC, and Key APIs
*************************************

LwMQ provides a set of fast hashing CRC, HMAC, and key generation
functions that can be used for various purposes such as message
integrity checks, record identity, and more.

Remember that every hash function is subject to collisions, and the
hashing functions provided by LwMQ are not cryptographic. They are
designed for speed and should not be used for security-sensitive
applications. However, they provide a fast solution for everyday
message integrity checks, enabling application to include checksums
and HMACs at a reasonable performance cost.

.. important::

    We want to emphasise once more that the HMAC and hashing functions
    provided with LwMQ are meant to offer a *fast* and reasonable way
    to include message integrity and authenticity at minimal performance
    cost.
    
    With them, you can detect a wide range of data corruptions
    and incorporate lightweight message integrity and authentication,
    through a pre-shared key, to verify that a message was sent by a
    trusted party (knowing the pre-shared key) and has not been tampered
    with during transmission.

    Exchanging and storing keys are outside of the scope of LwMQ.

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

Fast CRC Functions
------------------

The Crc16 function produces a T10-DIF CRC. The Crc32 function produces
a CRC-32 ISO/HDLC. The Crc64 function produces a CRC-64/XZ (CRC-64/GO-ECMA).

The resulting CRCs are meant to be used for message data corruption/validation
checks. Each CRC correspond to some CRC standard but there are so many we had
to pick some. For generic CRC's not provided here, you can use any external
CRC library.

Picking a CRC size depends on your risk aversion. See https://en.wikipedia.org/wiki/Cyclic_redundancy_check for the theory.

As a rule of thumb, use Crc16 for quick integrity checks on small messages (up to a
few KBs) and Crc32 otherwise (up to several MBs) then Crc64 for huge messages, or
switch to 64 or 128 bit hashes for added peace of mind at the expense of performances,
or HMACs if you want to authentify the sender (by the fact they know a secret key) in
addition to verifying the message integrity through a 128-bit hash.

A HMAC is nothing more than an encrypted hash: if they don't know the key, it will
be super-hard for them to generate the correct hash for a particular payload.

On de receiving side, knowing the payload, the HMAC, and the pre-shared secret
key, it is easy so see that they either didn't know the key, or that the message
content was altered during transmission, as they cannot change the content and
fix the hash without also knowing the secret key.

Some security architects advocate that authentication or encryption must be
super-strong or not be used at all. Here at LwMQ we think that a little security
is better than no security. We also believe in the `defense-in-depth`_ principle
where multiple layers of security are used conjointly to form a better defense.

.. _defense-in-depth: https://en.wikipedia.org/wiki/Defense_in_depth_(computing)

.. code:: cpp

    LMQAPI
    LmqComputeCrc16 (
        PCVOID Buffer,
        LengthBytes,
        PUINT16 Crc16
        );

    LMQAPI
    LmqComputeCrc32 (
        PCVOID Buffer,
        SIZE_T LengthBytes,
        PUINT32 Crc32
        );

    LMQAPI
    LmqComputeCrc64 (
        PCVOID Buffer,
        SIZE_T LengthBytes,
        PUINT64 Crc64
        );

Fast Hashing Functions
----------------------

They hash functions are not cryptographic and should not be used for
strong security purposes.

The hash comparison function is nevertheless designed to be resistant
to timing attacks by taking the same amount of time regardless of how
many bytes match between the two hashes.

.. code:: cpp

    LMQAPI
    LmqHashByteArray64 (
        PCVOID Buffer,
        SIZE_T LengthBytes,
        PUINT64 Hash
        );

    LMQAPI
    LmqHashByteArray128 (
        PCVOID Buffer,
        SIZE_T LengthBytes,
        PLMQ_HASH Hash
        );

    LMQAPI_(BOOL)
    LmqAreEqualHashes (
        PCLMQ_HASH Hash1,
        PCLMQ_HASH Hash2
        );

    //
    // C++ operator overloads for fast constant-time 128-bit hashes comparisons.
    //

    bool operator==(const LMQ_HASH& Hash1, const LMQ_HASH& Hash2);

    bool operator!=(const LMQ_HASH& Hash1, const LMQ_HASH& Hash2);

HMAC Functions
--------------

The encryption and hashing algorithms used to compute HMACs in LwMQ are meant
to be fast as their primary design goal. The cipher used is a weakened (lower
round count) version of an otherwise secure algorithm tuned for excellent
performance on modern processors.

The intent is to provide a way for security architects to include hash-based
message authentication codes (HMAC) in their LwMQ messaging payload with reasonabe
impact on performance, knowing the data links are supposed to be relatively secure
within the realms of applicability of LwMQ.

You can protect the keys in memory using LmqProtectKey(). In that case, you can
pass the protected key to LmqComputeHMAC() and LmqVerifyHMAC() provided that you
set the ``IsProtectedKey`` parameter to TRUE. The functions take care of unprotecting
a copy of the key and of properly erasing the unprotected copy when no longer needed.
This reduces the chances of key exposure by limiting the time the unprotected
key is present in memory. When a key is no longer needed, including when exiting
your program, securely erase any and all keys from memory using ``LmqEraseKey()``

For highly secure cryptographic operations, consider using well known facilities
such as `CryptoAPI`_, Crypto Next Generation (`CNG`_), or `OpenSSL`_ externally.

.. _CryptoAPI: https://learn.microsoft.com/en-us/windows/win32/seccrypto/cryptoapi-system-architecture
.. _CNG: https://docs.microsoft.com/en-us/windows/win32/seccng/cng-portal
.. _OpenSSL: https://www.openssl.org/

Fast One-shot HMAC Functions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This set of functions is meant for cases where only a few HMACs need to be computed
or verified, and the caller does not mind paying the cost of key setup every time.

This simplicity comes at some preformance cost.

.. code:: cpp

    LMQAPI
    LmqComputeHMAC (
        PCVOID Buffer,
        SIZE_T LengthBytes,
        PCLMQ_KEY Key,
        BOOL IsProtectedKey,
        PLMQ_HMAC HMAC
        );

    LMQAPI
    LmqVerifyHMAC (
        PCVOID Buffer,
        SIZE_T LengthBytes,
        PCLMQ_KEY Key,
        BOOL IsProtectedKey,
        PCLMQ_HMAC HMAC
        );

Fast HMAC Functions
^^^^^^^^^^^^^^^^^^^

This set of functions is meant for cases where many HMACs need to be computed
or verified with the same key, and the caller wants to pay the cost of key
setup only once.

The callers typically calls LmqInitHMACEx() once then calls LmqComputeHMACEx()
or LmqVerifyHMACEx() as many times as needed, then calls LmqDestroyHMACEx()
to free the context when no longer needed, for example at program exit.

You can protect the keys using LmqProtectKey(). In that case, you can pass the
protected key to LmqInitHMACEx() provided that you set the ``IsProtectedKey``
parameter to TRUE. The function will unprotect a copy of the key internally
and properly erase it when no longer needed.

Additionally, the context is encrypted when using a protected key. This helps
protect against memory dumps saving the decryption context intact by reducing
the time where the context is unencypted in memory.

The function uses a copy of the context and decrypts it, then safely disposes
of the copy when no longer needed. Note, however, that encryption/decryption
of the key and context incurs a performance penalty, and that there is still
a narrow window of opportunity (some microseconds) for an attacker to dump
the memory while the context is unencrypted or the key unprotected.

If you use protected keys, it is NOT recommended to unprotect the key prior
to calling LmqInitHMACEx() with the ``IsProtectedKey`` parameter set to FALSE,
as in this case the context will not be encrypted. 

.. code:: cpp

    LMQAPI
    LmqInitHMACEx (
        PCLMQ_KEY Key,
        BOOL IsProtectedKey,
        PLMQ_HMAC_CONTEXT Context
        );

    LMQAPI
    LmqComputeHMACEx (
        PCVOID Buffer,
        SIZE_T LengthBytes,
        PCLMQ_HMAC_CONTEXT Context,
        PLMQ_HMAC HMAC
        );

    LMQAPI
    LmqVerifyHMACEx (
        PCVOID Buffer,
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

This function provides a fast way to compare HMACs, which can be used
to verify that a computed HMAC matches an expected value.

The function is designed to be resistant to timing attacks by taking the
same amount of time regardless of how many bytes match between the two HMACs.

.. code:: cpp

    LMQAPI_(BOOL)
    LmqAreEqualHMACs (
        PCLMQ_HMAC HMAC1,
        PCLMQ_HMAC HMAC2
        );

    //
    // C++ operator overloads for fast constant-time HMAC comparisons.
    //
    
    bool operator==(const LMQ_HMAC& HMAC1, const LMQ_HMAC& HMAC2)

    bool operator!=(const LMQ_HMAC& HMAC1, const LMQ_HMAC& HMAC2)

Keys Functions
--------------

The key generation functions allow you to create ``LMQ_KEY`` structures
from ASCII and Unicode strings, or byte arrays, and to generate RFC
4122 Type 4 random keys.

The keys can then be used for HMAC computations, LwMQ Cache keys, or
other purposes, and can also be protected/unprotected in memory using
``LmqProtectKey()`` and ``LmqUnprotectKey()``, respectively.

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

Entropy Generation Functions
----------------------------

This function fills the provided ``LMQ_ENTROPY`` structure with random data
that can be used as a source of entropy for various purposes, such as
generating random keys or nonces.

The quality of the entropy provided by this function depends on the
underlying implementation and the platform's capabilities.

It is designed to be fast and suitable for general use cases, but it may not
be suitable for cryptographic purposes where high-quality randomness is
required.

.. code:: cpp

    LMQAPI
    LmqMakeEntropy (
        PLMQ_ENTROPY Entropy
        );

Key Protection Functions
------------------------

Those functions allow you to protect and unprotect keys in memory for use
within a particular process instance's lifetime, which can help mitigate
the risk of key exposure through memory dumps.

When you protect a key using ``LmqProtectKey()``, the key is encrypted in place. You
can then pass the protected key to the HMAC functions with the ``IsProtectedKey``
parameter set to TRUE, and the functions will take care of unprotecting a copy
of the key and properly erasing the unprotected copy when no longer needed.

You can also unprotect a key using ``LmqUnprotectKey()`` if you need to use the
unprotected key for other purposes, but be aware that this increases the risk
of key exposure. The key can be re-protected by calling ``LmqProtectKey()`` on
it again.

When you are done with a key, you should call ``LmqEraseKey()`` to securely erase
the key, protected or not, from memory.

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


Hex String Conversion Functions
-------------------------------

The hexadecimal conversion functions quickly convert
a GUID or a byte array into a null-terminated hex string
representation, either ASCII or UNICODE.

The GUID conversion functions have two variants, with
or without enclosing braces.

The hexadecimal letters 'a' to 'f' are always lowercase,
and the GUID conversion functions handle the peculiar
ordering and dash separators used in text-form GUIDs.

Their output is identical to the output produced by the
StringFromGUID() and StringFromCLSID() functions from
the Windows API except the lower casing and optional braces.

The casing debate is settled by `ITU-T X.667`_ and `RFC 4122`_
which stipulate that UUIDs/GUIDs must be lowercase, although
all processors must accept uppercase as input. The lowercase
convention is prevalent in Unix/Linux circles as the output
produced by the `uuidgen`_ utility.

.. _ITU-T X.667: https://www.itu.int/en/ITU-T/asn1/Pages/UUID/uuids.aspx
.. _RFC 4122: https://www.rfc-editor.org/rfc/rfc4122
.. _uuidgen: https://www.unix.com/man_page/redhat/1/uuidgen/

If you ever need to compare *well-formed* GUIDs in text form
then consider LmqAreEqualStringGuids() as described below.

The application must supply an appropriately sized
buffer to receive the strings. The LwMQ SDK headers
contain exact details regarding the buffer sizes.

Fast GUID/UUID To String Conversion
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Quickly produce the dashed "registry format" with or without surrounding braces.

.. important::

    The caller must supply a buffer large enough to hold the result.

.. code:: cpp

    LMQAPI_VOID
    LmqGuidToStringA (
        REFGUID Guid,
        PCHAR Out // 37 ASCII chars
        );

    LMQAPI_VOID
    LmqGuidToStringW (
        REFGUID Guid,
        PWCHAR Out // 37 UNICODE chars (74 bytes)
        );

    LMQAPI_VOID
    LmqGuidToBracedStringA (
        REFGUID Guid,
        PCHAR Out // 39 ASCII chars
        );

    LMQAPI_VOID
    LmqGuidToBracedStringW (
        REFGUID Guid,
        PWCHAR Out // 39 UNICODE chars (78 bytes)
        );

    //
    // Brace-insensitive and case-insensitive string
    // GUID equality comparison functions. These are
    // "fixed-function" functions that assume the
    // strings are well-formed GUID strings of proper
    // length. The functions don't check for length
    // or zero termination but simply skip any leading
    // opening brace, then perform a case-insensitive
    // comparison on the 36 characters that follow without
    // much regards for anything other than equality.
    //        

    LMQAPI_(BOOL)
    LmqAreEqualStringGuidsA (
        const CHAR* StringGuid1,
        const CHAR* StringGuid2
        );

    LMQAPI_(BOOL)
    LmqAreEqualStringGuidsW (
        const WCHAR* StringGuid1,
        const WCHAR* StringGuid2
        );

    #ifdef UNICODE
    #define LmqGuidToString LmqGuidToStringW
    #define LmqGuidToBracedString LmqGuidToBracedStringW
    #define LmqAreEqualStringGuids LmqAreEqualStringGuidsW
    #else
    #define LmqGuidToString LmqGuidToStringA
    #define LmqGuidToBracedString LmqGuidToBracedStringA
    #define LmqAreEqualStringGuids LmqAreEqualStringGuidsA
    #endif

Fast Generic Hex Conversion
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Quickly convert byte extents of any length to hex representation,
useful for printing hashes, CRCs, keys, and other binary data
in human-readable hexadecimal representation.

The hex conversion functions are not suitable to convert
little-endian integers (uint16, uint32, uint64) to their
hex representation as their byte ordering requires special
consideration.

.. important::

    The caller must supply a buffer large enough to hold the result.

.. code:: cpp

    LMQAPIIMP_VOID
    LmqBytesToHexStringA (
        const BYTE* Bytes,
        const SIZE_T Count,
        PCHAR Out // ((Count * 2) + 1) ASCII chars
        );

    LMQAPIIMP_VOID
    LmqBytesToHexStringW (
        const BYTE* Bytes,
        const SIZE_T Count,
        PWCHAR Out// ((Count * 2) + 1) UNICODE chars (((Count * 2) + 1) * 2 bytes)
        );

    #ifdef UNICODE
    #define LmqBytesToHexString LmqBytesToHexStringW
    #else
    #define LmqBytesToHexString LmqBytesToHexStringA
    #endif

.. note::

    You can make safe versions of those functions yourself,
    for example, knowing the number of characters written
    by the GUID conversions functions is fixed, you could
    easlily make a safe wrapper, e.g.:

    .. code:: cpp

        HRESULT
        SafeGuidToString (
            REFGUID Guid,
            PTCHAR Out,
            SIZE_T BufferSizeCch
            )
        {
            //
            // The functions always write 37 characters.
            //

            if (BufferSizeCch < (37 * sizeof(TCHAR)))
            {
                RETURN_WIN32(ERROR_INSUFFICIENT_BUFFER);
            }

            LmqGuidToString(Guid,
                            Out);

            return S_OK;
        }

    Alternatively, you could make safe macros with an assert in debug builds:

    .. code:: cpp

        #define SafeGuidToString(Guid, Out, Cch) do { assert(Cch >= (37 * sizeof(TCHAR))) LmqGuidToString(Guid, Out); } while(0,0)

    As a side note, Cch means "count of characters," as opposed to Cb, "count of bytes."
