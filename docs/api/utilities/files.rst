*******************
LwMQ File Functions
*******************

The file API consists of only one function at this time, ``LmqCleanupFiles()``,
which is a utility function that can be used to delete files matching given
file specifications within a given path, with various options for controlling
the behavior of the cleanup operation.

It is designed for server and cloud scenarios where large amounts of files
(millions) need to be scanned and some removed based on name matching or age,
and it is optimized for reliable performance, with a best effort approach
to deleting files and folders as it scans them, without the need to first
build an in-memory representation of the entire folder structure or the
list of files to be removed.

As such, it performs well where other approaches fail in presence of
very large numbers of files and folders, deep folder structures, and extended
length paths.

The function can delete files matching a list of file specifications in one
pass, for example ``"*.tmp;*.dmp;*.log"`` older than 30 days, and it can be
configured to ignore errors when trying to delete files and folders.

Additionally, a *maximum run time* can be specified to cap the time spent
cleaning up, which is useful in scenarios where the cleanup operation is
performed periodically and it is desirable to avoid long cleanup runs that
could negatively impact the system's performance.

Finally, the function optionally provides detailed statistics about the cleanup
operation, such as the number of files considered and removed, the number of
folders visited and removed, the total size of the files removed, and the
elapsed time, useful for monitoring and tuning the cleanup process and/or
logging to telemetry.

C and C++ Header File
=====================

.. code:: cpp

   #include <api-lwmq-files.h>

Binaries
========

.. code:: cpp

   api-lwmq-files-1-0.dll

Dependencies
============

.. code:: cpp

   api-lwmq-time-1-0.dll

Overview
========

Types
-----

.. code:: cpp

   typedef struct _LMQ_CLEANUPFILES_STATS
   {
      UINT64 FilesRemovedSizeBytes;
      UINT32 FilesConsidered;
      UINT32 FilesRemoved;
      UINT32 FoldersVisited;
      UINT32 FoldersRemoved;
      UINT32 ElapsedTimeMs;
   }
   LMQ_CLEANUPFILES_STATS, *PLMQ_CLEANUPFILES_STATS;

Constants
---------

.. code:: cpp

   #define LMQ_CLEANUPFILES_RECURSE_SUBFOLDERS           0x00000001
   #define LMQ_CLEANUPFILES_DELETE_READONLY_FILES        0x00000002
   #define LMQ_CLEANUPFILES_DELETE_EMPTY_SUBFOLDERS      0x00000004
   #define LMQ_CLEANUPFILES_DELETE_SUBFOLDERS_LEFT_EMPTY 0x00000008
   #define LMQ_CLEANUPFILES_DELETE_ROOT_FOLDER_IF_EMPTY  0x00000010
   #define LMQ_CLEANUPFILES_ALSO_AGE_FOLDERS             0x00000020

   #define LMQ_CLEANUPFILES_IGNORE_FILE_DELETION_ERRORS  0x00000100
   #define LMQ_CLEANUPFILES_IGNORE_FOLDER_ACCESS_ERRORS  0x00000200

   #define LMQ_CLEANUPFILES_TRY_DELETE_EVERYTHING       (LMQ_CLEANUPFILES_RECURSE_SUBFOLDERS | \
                                                         LMQ_CLEANUPFILES_DELETE_READONLY_FILES | \
                                                         LMQ_CLEANUPFILES_DELETE_EMPTY_SUBFOLDERS | \
                                                         LMQ_CLEANUPFILES_DELETE_ROOT_FOLDER_IF_EMPTY | \
                                                         LMQ_CLEANUPFILES_IGNORE_FILE_DELETION_ERRORS | \
                                                         LMQ_CLEANUPFILES_IGNORE_FOLDER_ACCESS_ERRORS)

   #define LMQ_CLEANUPFILES_DEFAULT                     (LMQ_CLEANUPFILES_RECURSE_SUBFOLDERS | \
                                                         LMQ_CLEANUPFILES_DELETE_READONLY_FILES | \
                                                         LMQ_CLEANUPFILES_DELETE_SUBFOLDERS_LEFT_EMPTY | \
                                                         LMQ_CLEANUPFILES_IGNORE_FILE_DELETION_ERRORS | \
                                                         LMQ_CLEANUPFILES_IGNORE_FOLDER_ACCESS_ERRORS)

Functions
---------

.. code:: cpp

   LMQAPI
   LmqCleanupFiles (
      PCWSTR Path,
      PCWSTR FileSpecList,
      UINT64 MinimumAge,
      DWORD Flags,
      const UINT32 TimeoutMs,
      PLMQ_CLEANUPFILES_STATS Stats
      );

Details
=======

.. c:function:: LMQAPI LmqCleanupFiles(PCWSTR Path, PCWSTR FileSpecList, UINT64 MinimumAge, DWORD Flags, const UINT32 TimeoutMs, PLMQ_CLEANUPFILES_STATS Stats);

    This function deletes files that match a given file specification
    within a given path. Optionally, it can recurse into subfolders and
    when doing so it will remove any subfolder(s) left empty. The function
    supports extended length paths (up to 32K) and recurse into very deep
    folder structures. It is designed for server scenarios where large amounts
    of files (millions) need to be scanned and some removed.
    
    The function uses a (low) fixed amount of memory independent of the number
    of files and folders, and it is optimized for performance, with a best effort
    approach to deleting files and folders as it scans them, without the need
    to first build an in-memory representation of the entire folder structure
    or the list of files to be removed.

   :param Path: Provides the path to the folder where the cleanup operation is to be performed. The path must exist and cannot contain wildcards.

   :param FileSpecList: Provides an optional DOS file specification to match files against. If nullptr, all files are considered. The function accepts either a single filespec like L"*.tmp" or multiple filespecs in a semi-colon separated list, e.g. L"*.tmp;*.mdmp;abc_????.log" For best performance, do not terminate the list with a semi-colon.

   :param MinimumAge: Provides the minimum age in 100ns units ("clunks" or NT Time) that a file must have since last written in order to be considered for deletion. If zero, all files are considered.

   :param Flags: Control the function behavior. The following self-explanatory flags are supported:

                         LMQ_CLEANUPFILES_RECURSE_SUBFOLDERS
                         LMQ_CLEANUPFILES_DELETE_READONLY_FILES
                         LMQ_CLEANUPFILES_DELETE_EMPTY_SUBFOLDERS
                         LMQ_CLEANUPFILES_DELETE_SUBFOLDERS_LEFT_EMPTY
                         LMQ_CLEANUPFILES_DELETE_ROOT_FOLDER_IF_EMPTY
                         LMQ_CLEANUPFILES_IGNORE_FILE_DELETION_ERRORS
                         LMQ_CLEANUPFILES_IGNORE_FOLDER_ACCESS_ERRORS

                     Aggregate flags:

                         LMQ_CLEANUPFILES_DEFAULT
                         LMQ_CLEANUPFILES_TRY_DELETING_EVERYTHING

   :param TimeoutMs: Provides a maximum run time for the function, enabling to cap the time spent cleaning up, in milliseconds. Pass INFINITE to disable the timeout feature.

   :param Stats: Optional pointer to an LMQ_CLEANUPFILES_STATS structure.


   


