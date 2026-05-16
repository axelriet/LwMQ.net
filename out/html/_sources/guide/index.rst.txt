###############
LwMQ User Guide
###############

Introduction
============

LwMQ is a systems and platform architect's LEGO® set, born
from a simple observation: there is a gap in message-oriented
inter-process communication system's offerings.

LwMQ provides *extremely fast* inter-process messaging, *extremely fast*
in-memory caching with special attention to high-contention scenarios,
a persistent key-value store as well as supporting features such as
hashing, CRC, HMAC and key and entropy generation, data compression,
and more.

Everything in LwMQ is lightweight and optimized for the best possible
performance on modern hardware, and exposed through a simple and
consistent API.

LwMQ is designed to be used in a wide variety of scenarios, from
locally distributed applications to networked applications, and
from `IoT`_ / embedded SoCs to workstations to datacenters, and even
in virtualized environments.

- Samples: `LwMQ Samples (GitHub)`_
- SDK: https://www.lwmq.net

.. _IoT: https://learn.microsoft.com/en-us/windows/iot/
.. _LwMQ Samples (GitHub): https://github.com/axelriet/LwMQ.net/tree/main/samples


The entire binary distribution of LwMQ weights about 4MB and fits
comfortably on any device capable of running any general-purpose SKU
of Windows 10, Windows 11, Windows Server 2022, or later, while the
redistributable installer weights less than 600KB.

LwMQ's performance is best-in-industry when not industry-defining, on
every aspect, and set a new standard of excellence. LwMQ is designed to be
used in performance-critical and mission-critical long-running scenarios,
with a strong emphasis on correctness and reliability.

Axel Rietschin Software Development, LLC is a Microsoft Partner member
of the Microsoft Windows Hardware Compatibility Program (WHCP) and
is committed to ensuring LwMQ's compatibility with Windows and its ecosystem,
as well as to providing the best possible experience for our users.

.. image:: ../_static/img/Windows_HW_h_rgb_blk_5mm.png
   :scale: 50%
   :alt: Microsoft Windows Hardware Compatibility Program (WHCP) Logo
   :target: https://learn.microsoft.com/en-us/windows-hardware/design/compatibility/

User Guide Sections
===================

   .. toctree::
      :maxdepth: 1

      messaging
      caching
      supporting-features
      implementation
      philosophy
      api
      bugcheck
      tracelogging
      debugging
      exceptions
      telemetry
      architecture
      getting-started
      installation
      licensing
      platform
      requirements

Software Development Kit
========================

   .. toctree::
      :maxdepth: 1

      sdk
      samples
      tools


