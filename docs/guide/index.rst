###############
LwMQ User Guide
###############

Introduction
============

LwMQ is a systems and platform architect's LEGO® set, born
from a simple observation: there is a gap in message-oriented
inter-process communication system's offerings.

LwMQ provides *fast* inter-process messaging, caching, and key-value
storage as well as supporting features such as hashing, HMAC and
key generation, data compression, and more. Everything is
lightweight and optimized for the best possible performance on
modern hardware, and exposed through a simple and consistent API.

LwMQ is designed to be used in a wide variety of scenarios, from
locally distributed applications to networked applications, and
from `IoT`_ / embedded SoCs to workstations to datacenters, and even
in virtualized environments.

Samples: https://github.com/axelriet/LwMQ.net/tree/main/samples

SDK: https://www.lwmq.net

.. _IoT: https://learn.microsoft.com/en-us/windows/iot/

The entire binary distribution of LwMQ is less than 4MB and fits
comfortably on any device capable of running any general-purpose SKU
of Windows 10, Windows 11, Windows Server 2022, or later, while the redistributable
installer weights less then 600KB.

LwMQ's performance is best-in-industry on every aspect, and it
is designed to be used in performance-critical and mission-critical
long-running scenarios, with a strong emphasis on correctness and
reliability.

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


