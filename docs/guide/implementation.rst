##############
Implementation
##############

LwMQ is written in the C programming language following strict
disciplines mostly found in kernel development and mission-critical,
long-running software. It is suitable for *always on* appliance-type
software with indefinite uptimes.

The C code is compiled as C++ to benefit from stricter type checking
of the latter language, a technique now used in the Windows kernel
and other critical software projects, and the project takes maximum
advantage of static analysis for compile-time defect detection.

The code is designed to be robust and bomb-proof, with a strong
emphasis on correctness and reliability, while providing
best-in-industry performance on every aspect.

Some light touches of proper C++ are used here and there, for example
the finite-state machine that governs the state of the transports
is best implemented in C++ [`GoF`_, p. 305ff]

.. _GoF: https://a.co/d/02cPzhzs

Small portions are written in assembly language and the code
often uses vector instructions (SIMD intrinsics) where appropriate.

LwMQ *requires* AVX-2 instructions (Haswell, Ryzen, or later) and
performs exceptionally well on contemporary hardware.

Some features, including compression and encryption and some hashing
functions, make use of specialized CPU instructions and can take advantage
of recent advances, such as AES-NI and AVX-512, on suitable hardware
(e.g. Skylake or later) thanks to runtime dispatching. LwMQ squeezes
every bit of performance from your hardware!

