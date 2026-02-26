.. |br| raw:: html

   <br />

*************
API Reference
*************

Init/Term
=========

Macros
------

.. c:macro:: LMQ_CURRENT_API_VERSION

Functions
---------

.. c:function:: LMQAPI LmqInitialize(WORD VersionRequested)

    |br|
    Initializes LwMQ.

    :param VersionRequested: Pass the supplied LMQ_CURRENT_API_VERSION macro.

    .. note:: This function must be called before any other LwMQ function. Multiple calls are allowed but must be matched by a corresponding call to LmqTerminate().
    |br|

.. c:function:: LMQAPI LmqTerminate()

    |br|
    Shuts down LwMQ and deallocate internal ressources.
    
    .. note:: This function must be called once for each calls made to LmqInitialize().
    |br|

.. code:: c

    #include <api-lwmq-messaging.h>

    HRESULT hr = LmqInitialize(LMQ_CURRENT_API_VERSION);

    // ...

    hr = LmqTerminate();

Messages
========

Messages are structured entities composed of one or more data frames.

Types
-----

.. c:type:: LMQ_MESSAGE;

    An opaque message instance.

.. c:type:: PLMQ_MESSAGE

    A pointer to an opaque message instance.


Queues
======

Transports
==========

Channels
========
