INFORMATION
===========

This program can set up the UBX GPS chip to obtain
faster fix after powering on (TTFF). To do this it
needs AGPS information.

The AGPS information can be grabbed from GPS chip
when it has fix (reports valid position). Do:

    ./gta02-agps.py --dump agpsdata < /dev/ttySAC1 > /dev/ttySAC1

This will save AGPS data read from chip into the
file "agpsdata".

This data can be loaded into the GPS after it is
stopped and started again. This should improve TTFF.

To load data from agpsdata file issue:

    ./gta02-agps.py --load agpsdata < /dev/ttySAC1 > /dev/ttySAC1

PURPOSE
=======

This code is quite functional, but is published
mostly as a reference for understanding AGPS
load/dump possibilities for UBX chips.

DEPENDENCIES
============

This code depends on standard python modules,
no serial port libraries are needed, since we
use /dev/ttySAC1 as normal file (it should be
configured right by the system).

PROBLEMS
========

Sometime (especially when using --reset and
--load at the same time) aid data request is
somehow missed by reading process and thus is
not supplied to the device. In real-life
situations you want to --load only on a freshly
powered on device, you won't supply --reset
and as a result this problem should not happen.

AGPS data is saved as Python representation of
Python dictiorary with data. This should be
probably saved in more portable format.

LICENSE
=======

Code licensed on GPLv2 or later. Written by:

(C) 2010 Piotr Gabryjeluk <piotr@gabryjeluk.pl>

Based on Open GPS Daemon by

(C) 2008 Jan 'Shoragan' Lübbe <jluebbe@lasnet.de>
(C) 2008 Daniel Willmann <daniel@totalueberwachung.de>
(C) 2008 Openmoko, Inc.
licensed on GPLv2 or later

