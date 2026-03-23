############
Installation
############

LwMQ is deployed through standard Windows Installer (MSI) packages, which
can be easily installed and managed on any Windows system.

Installers
""""""""""

LwMQ is packaged in two distinct MSIs: one for the SDK, and one
for the redistribuable runtime components.

.. table::
   :width: 95%
   :widths: 33, 66
   :align: left

   ==================  =============================================
   Package Name        Description
   ==================  =============================================
   lwmq.sdk.setup.msi  Development files for applications using LwMQ
   ==================  =============================================

This MSI installation database deploys the development files (headers,
libraries, debug symbols, samples) needed to develop applications that use LwMQ.

The SDK *also* deploys the runtime components, so installing the SDK
is a one-stop solution for development and CI/CD build machines. However,
the SDK is not meant to be deployed on production systems.

Finally, the SDK includes the redistributable **lwmq.setup.msi** package
so the application being built, and in particular its setup system, can
reference the runtime from a well-known location and include it with
the application.

.. table::
   :width: 95%
   :widths: 33, 66
   :align: left

   ==============  ==============================================
   Package Name    Description
   ==============  ==============================================
   lwmq.setup.msi  Runtime components for applications using LwMQ
   ==============  ==============================================

This MSI installation database deploys the runtime components of LwMQ to
the target computer. Every application *should* deploy the runtime package
to ensure the correct version of the runtime components. The LwMQ installer
takes care of counting references and to make sure the latest version of
the runtime components is present on the system.

Similarly, applications *should* uninstall the runtime package when they
are uninstalled: the Windows Installer subsystem takes care of removing
the components when they are no longer needed after the last application
that installed them gets uninstalled.

Alternatively, system administrators can also install the runtime package
in their system image or application layer and manage it through their
standard fleet update and maintenance process, for example through
`Microsoft Configuration Manager`_ or other endpoint management system.

.. _Microsoft Configuration Manager: https://learn.microsoft.com/en-us/intune/configmgr/

The installer and all binary files it contains are digitally signed
using our DigiCert Extended Validation (EV) code signing certificate.

