##############
LwMQ SDK Tools
##############

.. table::
   :width: 95%
   :widths: 33, 66
   :align: left

   ================  ========================================================================
   Variable          Description
   ================  ========================================================================
   client_thr.exe    Througput test (client)
   ----------------  ------------------------------------------------------------------------
   server_thr.exe    Througput test (server)
   ================  ========================================================================

.. important::
   
   The supplied test tools, ``client_thr.exe`` and ``server_thr.exe`` require
   the ``SeCreateGlobalPrivilege`` when using the IPC protocol, as described on
   the :doc:`../transports/ipc` page.

   The IPC protocol is the ONLY transport available in the March 23, 2026 1.0.0.5 ALPHA (and DEMO)
   release of the SDK.
