########
LwMQ SDK
########

The LwMQ SDK contains the files necessary for developement.

Installation Folder
===================

By default, the SDK install itself at ``%ProgramFiles%\LwMQ.SDK``

Environment Variables
=====================

The LwMQ SDK installer creates the following environment variables:

.. table::
   :width: 95%
   :widths: 33, 66
   :align: left

   ================  ========================================================================
   Variable          Description
   ================  ========================================================================
   LMQ_SDK_ROOT      Points to the SDK's root folder
   ----------------  ------------------------------------------------------------------------
   LMQ_SDK_INCLUDE   Points to the C header files
   ----------------  ------------------------------------------------------------------------
   LMQ_SDK_LIB       Points to the import libraries
   ----------------  ------------------------------------------------------------------------
   LMQ_SDK_SYMBOLS   Points to the debug symbols (PDB)
   ================  ========================================================================

.. important::

   The SDK installer modifies the environment variables.
   
   You must restart any command prompt or Visual Studio
   instance to see the new environment variables after the
   SDK installation.
   
   The enviroment variables are referenced from the build
   files to find the 'include' and 'lib' subfolders.

Subfolders
==========

The LwMQ SDK installer creates the following subfolders within the SDK root:

.. table::
   :width: 95%
   :widths: 33, 66
   :align: left

   =============  ========================================================================
   Subfolder      Description
   =============  ========================================================================
   inc            Header files needed to develop applications
   -------------  ------------------------------------------------------------------------
   lib            Import libraries needed to link applications
   -------------  ------------------------------------------------------------------------
   licenses       Open-source licenses for third-party components used in LwMQ
   -------------  ------------------------------------------------------------------------
   pdb            Debug symbols
   -------------  ------------------------------------------------------------------------
   redist         Redistributable files (runtime installer)
   -------------  ------------------------------------------------------------------------
   tools          Command-line test tools, and unit + scenario tests
   =============  ========================================================================

Running Tests
=============

The LwMQ SDK ships with the comprehensive unit-test suite and scenario tests
to validate the functionality of the SDK on developer's and build machines.

Our internal test suite comprises more tests, notably long-running stress tests
and debug-only tests that calle API functions that arent part of the public API.

Remember to point your debugger to the SDK's PDB folder (``%LMQ_SDK_SYMBOLS%``)
which contains PDBs matching the redistributable binaries.

You can also use our symbol server at ``https://www.lwmq.net/symbols`` which also
contains symbols for the SDK tools.

See the 'tools' subfolder for more details, in particular RunningTests.txt

.. table::
   :width: 95%
   :widths: 33, 66
   :align: left

   ======================  ========================================================================
   File                    Description
   ======================  ========================================================================
   LwMQ.UnitTests.dll      Unit-test suite for LwMQ (Visual Studio Test)
   ----------------------  ------------------------------------------------------------------------
   LwMQ.ScenarioTests.exe  Scenario test suite for LwMQ (GoogleTest)
   ======================  ========================================================================


