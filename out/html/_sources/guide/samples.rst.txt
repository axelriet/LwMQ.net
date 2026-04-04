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

This commmand clones the GitHub repository in a subfolder named ``LwMQ.net``


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