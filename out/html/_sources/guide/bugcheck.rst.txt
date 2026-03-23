#####################
BugChecks (Fail Fast)
#####################

LwMQ embraces a *fail fast* philosophy: when a critical error is detected,
the process will BugCheck (crash) immediately to prevent any potential
data corruption or security breach. This occurs when a clear programmer
error is detected, for example when passing an invalid object handle
to an API function. Another reason to fail fast is to capture the state
of the process as close as possible from the detection of the problem,
often making diagnosis far easier simply by having the right call stack.

This topic is always controversial in programming circles and subject
to heated debates, but the author firmly believes that in a low-level library
such as LwMQ, which is designed to be used in performance-critical and
mission-critical scenarios, it is better to fail fast (and loud!) rather
than silently continue in a potentially corrupted state.

LwMQ components does not fail fast for common errors, but are very strict
about correct usage. By making misuses immediately apparent, LwMQ helps
developers detect and fix bugs early in the development cycle, rather
than allowing them to manifest later in production where they can
cause more harm.

Validation
==========

Examples of regular error return during parameter validation:

.. code:: cpp

   RETURN_HR_IF_MSG(E_INVALIDARG,
                    BufferSizeBytes < 1,
                    "BufferSizeBytes must be at least 1.");

   RETURN_HR_IF_MSG(E_INVALIDARG,
                    MaximumSizeBytes == 0,
                    "MaximumSizeBytes must of > 0, realistically "
                    "some number of megabytes/gigabytes.");

There are approximately 300 locations across the codebase where LwMQ
performs parameter validation and checks for error conditions and
returns errors to the caller.

Fail Fast
=========

Example of a bugcheck for a critical programmer error:

.. code:: cpp

   if (LmqiIsValidWorkspace(WorkspaceInternal) == FALSE)
   {
      FAIL_FAST_HR_MSG(E_INVALIDARG,
                       "Invalid compression workspace handle.");
   }

Example of a bugcheck for a critical internal error (i.e. a condition
that should never occur regardless of circumstances, and is therefore
likely a bug that must be addressed):

.. code:: cpp

   if (LastEntry == nullptr)
         [[unlikely]]
   {
         FAIL_FAST_HR_MSG(E_UNEXPECTED,
                          "LastEntry should not be null.");
   }

There are approximately 150 locations across the codebase where LwMQ
performs critical validations and would bugcheck on fatal errors.
