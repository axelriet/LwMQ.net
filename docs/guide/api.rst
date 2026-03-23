###
API
###

The API follows a C-style design, with a flat API and
opaque types, and is designed to be easily callable from C,
C++, Rust, Go, Python and more. Wrappers and bindings can
also be created for almost any language.

The ABI (Application Binary Interface) remains stable between
releases, ensuring backward compatibility: new versions of
LwMQ will generally be drop-in replacements for any previous
version, meaning the API surface is stable and extensions
are made by adding new functions and types, rather than
modifying existing ones. Existing functions will *never* be
removed, ensuring as much as possible that older applications
can use newer versions of LwMQ unmodified and without breaking.

The naming convention is to prefix all API functions with "Lmq"
and all types with "\L\M\Q\_" (and "\P\L\M\Q\_" for pointers to types)
to avoid any naming collision with other APIs or libraries.

The naming format follows the PrefixVerbNoun structure in TitleCase,
with the verb describing the action performed by the function and
the noun describing the main entity the function operates on.

For example, LmqCreateChannel() creates a new communication channel,
LmqPostMessage() posts a message to a queue, and so on. No surprises.

Create verbs are complemented by Destroy verbs that free the
resources allocated by the Create functions. Open -> Close,
Allocate -> Free, and so on.

Save for a couple of rare exceptions, LwMQ's API calls return a status
code (HRESULT) indicating success or failure.
