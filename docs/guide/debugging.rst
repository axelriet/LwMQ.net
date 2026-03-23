#############
Debug Symbols
#############

We publish debug symbols for all LwMQ binaries to our
symbol server. Debugging tools can be configured to
automatically retrieve our public symbols when debugging
or profiling applications that use LwMQ.

The symbol server URL is:

.. code:: cpp

   https://www.lwmq.net/symbols

There is no user-browseable content at that address but
debugging tools know how to use it.

Visual Studio can be configured to use our symbol server by
adding it to the list of symbol servers in the debugging
options: Tools->Options->Debugging->Symbols.

Additionally, the debug symbols ship with the SDK for
direct reference of the latest symbol version from developer
machines.

The symbol server maintains a history of all published
versions of all runtime binaries and is suitable for
debugging, profiling, and crash dump analysis of your
applications from machines that don't have the LwMQ SDK
installed.
