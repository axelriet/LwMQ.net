################
LwMQ SDK Samples
################

Getting the samples
===================

Sample programs demonstrating various aspects of
LwMQ are available on the `GitHub page`_.

Supposing you are in a ``D:\GIT`` folder on your computer, run:

.. code::

    git clone https://github.com/axelriet/LwMQ.net.git

This command clones the GitHub repository in a subfolder named ``LwMQ.net``

Building the samples
====================

All samples use a Visual Studio 2022 or later solution file and are ready to be built and run.

You can download Visual Studio Community Edition for free from:

    https://visualstudio.microsoft.com/downloads/

You also need the latest LwMQ SDK to build the samples, which you can download from:

    https://www.lwmq.net

To install the SDK, double-click the ``lwmq.sdk.setup.msi`` installer and follow the prompts.

.. _GitHub page: https://github.com/axelriet/LwMQ.net

You can build the samples from the VS IDE or from the command line:

1. Start a ":command:`x64 Native Tools Command Prompt for VS 2022`" (from your start menu)
2. Go to the samples folder (e.g. ``D:\GIT\LwMQ.net\samples``) where ``LwMQ.Samples.sln`` resides.
3. Run:

.. code::

    msbuild /property:Configuration=Release LwMQ.Samples.sln

Output:

.. table::
   :width: 66%
   :widths: 44, 56
   :align: left

   =========================  ====================================
   What                       Where
   =========================  ====================================
   Built samples              ``.\bin`` subfolder
   -------------------------  ------------------------------------
   Temporary build artifacts  ``.\obj`` subfolder (can be deleted)
   =========================  ====================================

Running the samples
====================

.. important::

    The ipc:// transport used in some of the samples has security requirements, see the :doc:`../transports/ipc` section for details.

To run a sample, for example the MiniChat sample, do the following:   

1. Go to the ``.\bin\MiniChat\x64\Release`` subfolder.

2. Run two copies of the sample executable (if your command prompt runs elevated):
 
.. code::

       start MiniChat
       start MiniChat

If the command prompt does not run under an account that has the necessary
permissions to use the ``ipc://`` transport yet, you can start the sample elevated
using powershell (Windows may prompt you for permission):

.. code::

       powershell -Command "Start-Process MiniChat -Verb RunAs"
       powershell -Command "Start-Process MiniChat -Verb RunAs"

You are encouraged to modify the sample code and experiment with it
to learn how to use LwMQ.

If you made some experiment that you think is worth sharing, please consider
contributing it back to the samples repository on GitHub.

The samples are provided as-is and are not intended to be used as production code.

.. important::

    The samples are designed to be simple and easy to understand, and may not follow best practices for production code. They are intended for educational purposes only.

    If you run into *any* issue while working with LwMQ, always run your code in a debugger, for example the Visual Studio debugger, WinDbg, or WinDbgX.

    If you suspect a crash when running outside a debugger, install WinDbg as the default post-mortem crash debugger by running ``windbg /I`` (or ``WinDbgX /I``) from an elevated command prompt (the switch is forward slash immediately followed by a CAPITAL i letter, the casing is important!) and the debugger will pop up when any process crashes on your system.

    If you use Visual Studio, you can configure the debugger for post-mortem crash debugging from the Tools | Options | Debugging | Just-In-Time dialog. Then make sure to check Native plus any other relevant modes.
    
    You can only have one post-mortem debugger installed at any given time.

    Remember that our symbols are at http://www.lwmq.net/symbols and that you can configure your debugger to use this symbol server to get the best debugging experience.
    
    LwMQ's APIs return errors often accompanied with elaborate diagnostic messages that are *only* visible from inside a debugger.
    
    These messages can be very helpful to understand what went wrong and how to fix it. LwMQ goes to great lengths to provide useful diagnostics to help developers build robust applications.