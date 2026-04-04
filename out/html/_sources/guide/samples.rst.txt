################
LwMQ SDK Samples
################

Sample programs are available on the `GitHub page`_. Supposing you are in a ``D:\GIT`` folder, run:

    :command:`git clone https://github.com/axelriet/LwMQ.net.git`

This commmand will clone the GitHub repository in a subfolder named ``LwMQ.net``

All samples use a Visual Studio 2022 or later solution file and are ready to be built and run.

You can download Visual Studio Community Edition for free from:

    https://visualstudio.microsoft.com/downloads/

You also need the latest LwMQ SDK to build the samples, which you can download from:

    https://www.lwmq.net

To install the SDK, double-click the ``lwmq.sdk.setup.msi`` installer and follow the prompts.

.. _GitHub page: https://github.com/axelriet/LwMQ.net

You can build the samples from the IDE or from the command line:

1. Start a ":command:`x64 Native Tools Command Prompt for VS 2022`"
2. Go to the samples folder (e.g. ``D:\GIT\LwMQ.net\samples``) where ``LwMQ.Samples.sln`` resides.
3. Run:

    :command:`msbuild`

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

.. important::

    The ipc:// transport used in some of the samples has security requirements, see the :doc:`../transports/ipc` section for details.

To run a sample, for example the MiniChat sample, do the following:   

1. Go to the ``.\bin\MiniChat\x64\Debug`` subfolder.

2. Run two copies of the sample executable:
 
       :command:`start MiniChat`

       :command:`start MiniChat`

If the command prompt does not run under an account that has the necessary
permissions to use the ipc:// transport yet, you can start the sample elevated
using powershell:

       :command:`powershell -Command "Start-Process MiniChat -Verb RunAs"`
       
       :command:`powershell -Command "Start-Process MiniChat -Verb RunAs"`

