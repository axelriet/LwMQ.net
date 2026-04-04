########
LwMQ SDK
########

The LwMQ SDK contains the files necessary for developement.

Installation Folder
===================

By default, the SDK install itself at ``%ProgramFiles%\LwMQ.SDK``

Environment Variables
=====================

The SDK installer creates the following environment variables:

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
   LMQ_SDK_SYMBOLS   Points to the debug symbol files (PDB)
   ================  ========================================================================

.. important::

   The SDK installer modifies the environment. You must restart any command prompt or
   Visual Studio instance to see the new environment variables. The enviroment variables
   are referenced in the build files to find the include and lib subfolders.

Subfolders
==========

The SDK installer creates the following subfolders within the SDK root:

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
   pdb            Debug symbols
   -------------  ------------------------------------------------------------------------
   redist         Redistributable files (runtime installer)
   -------------  ------------------------------------------------------------------------
   tools          Command-line tools for testing LwMQ
   =============  ========================================================================
