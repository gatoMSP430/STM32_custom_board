Copyright (C) 2017, 2018, 2019, 2021 Theia Space, Universidad Politécnica de Madrid

This file is part of Theia Space's ESAT Wifi library.

Theia Space's ESAT Wifi library is free software: you can
redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

Theia Space's ESAT Wifi library is distributed in the hope that it
will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Theia Space's ESAT EPS library.  If not, see
<http://www.gnu.org/licenses/>.


ESAT Wifi library version 2.2.0

Wifi communications subsystem for ESAT.

This software runs in the ESAT Wifi board.  Use it with Arduino 1.8.0
or newer together with the Arduino core for ESP8266-based boards
(esp8266) 2.3.0 or a newer 2.x.y version and the ESAT utility library
(ESATUtil) version 2.1.0 or a newer 2.x.y version.  Use Generic
ESP8266 Module as the target board.

See the example Wifi program (examples/Wifi/Wifi.ino).  This program
uses the modules of the Wifi library.

The src/ directory contains the Wifi library (ESAT_Wifi) as well as
several subdirectories:


# ESAT_Wifi

This is the main library that provides the Wifi communications
functionality.


# ESAT_Wifi-peripherals directory

Peripherals of the ESAT Wifi board.


# ESAT_Wifi-telecommands directory

Telecommands of the ESAT Wifi board.


# ESAT_Wifi-telemetry directory

Telemetry packets of the ESAT Wifi board.
