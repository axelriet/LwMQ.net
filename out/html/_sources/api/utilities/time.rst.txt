*******************
LwMQ Time Functions
*******************

Nanosecond time difference and tick count functions.

C and C++ Header File
=====================

.. code:: cpp

   #include <api-lwmq-time.h>

Binaries
========

.. code:: cpp

   api-lwmq-time-1-0.dll

Dependencies
============

.. code:: cpp

   None

Overview
========

Types
-----

.. code:: cpp

    None

Time Functions
--------------

.. code:: cpp

   LMQAPI_(UINT64)
   LmqTimeElapsed (
      const FILETIME* const Then,
      const FILETIME* const Now
      );

   LMQAPI_(UINT64)
   LmqTimeElapsedNs (
      const UINT64 Then,
      const UINT64 Now
      );

   LMQAPI_(UINT64)
   LmqTimeElapsedNsSince (
      const UINT64 Then
      );

   LMQAPI_(UINT64)
   LmqGetTickCountNs (
      VOID
      );
