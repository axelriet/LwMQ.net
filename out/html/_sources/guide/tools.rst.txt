##############
LwMQ SDK Tools
##############

The LwMQ SDK ships with command-line test tools. See the 'tools' subfolder
for more details, in particular Tools.txt

.. table::
   :width: 95%
   :widths: 33, 66
   :align: left

   ================  ========================================================================
   Variable          Description
   ================  ========================================================================
   client_thr.exe    Throughput test (client)
   ----------------  ------------------------------------------------------------------------
   server_thr.exe    Throughput test (server)
   ================  ========================================================================

.. important::
   
   The supplied test tools, **client_thr.exe** and **server_thr.exe** require
   the **SeCreateGlobalPrivilege** when using the IPC protocol, as described on
   the :doc:`../transports/ipc` page.

   The IPC protocol is the ONLY transport available in the April 7, 2026 1.0.0.8 ALPHA DEMO release of the SDK.
