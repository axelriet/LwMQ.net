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

.. c:function:: LMQAPI LmqInitialize (WORD VersionRequested)
    
   Initializes LwMQ. This function must be called before any
   other LwMQ functions are called. Multiple calls are allowed
   but must be matched by a corresponding call to LmqTerminate().   

   :param VersionRequested: Pass the supplied LMQ_CURRENT_API_VERSION macro.

.. c:function:: LMQAPI LmqTerminate()

   Shuts down LwMQ and deallocate internal ressources. This function must be called once for each
   calls made to LmqInitialize().  

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
