###################
Tracing and Logging
###################

Note that LwMQ uses the error tracing/logging and reporting
macros provided by the `Window Internal (or Implementation) Libraries (WIL)`_ to
provide detailed error messages and context to the developer when a failure occurs,
which can be invaluable for debugging and troubleshooting. Most errors have
an associated message that provides succinct details about the specific error
condition, as illustrated above.

LwMQ also makes *modest* use of the Windows TraceLogging facilities to log
significant events which can be enabled and collected with tools such as
`Windows Performance Recorder (WPR) and Windows Performance Analyzer (WPA)`_
for in-depth analysis of the system's behavior, or simply viewed in
real-time with tools such as `TraceView`_.

.. _Window Internal (or Implementation) Libraries (WIL): https://github.com/microsoft/wil
.. _TraceView: https://learn.microsoft.com/en-us/windows-hardware/drivers/devtest/traceview
.. _Windows Performance Recorder (WPR) and Windows Performance Analyzer (WPA): https://learn.microsoft.com/en-us/windows-hardware/test/wpt/windows-performance-analyzer

The ETW provider GUID for the messaging component is:

.. code:: cpp

   //
   // "api-lwmq-messaging"
   //
   // {7da8eabe-76ce-556e-7a7e-63e75c6c78f5}
   //

