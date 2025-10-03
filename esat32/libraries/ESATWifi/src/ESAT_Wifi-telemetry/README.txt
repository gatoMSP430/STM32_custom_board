Copyright (C) 2018, 2019 Theia Space, Universidad Politécnica de Madrid

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


Wifi telemetry packet contents objects fill telemetry packets.  Each
telemetry packet contents object implements the
ESAT_CCSDSPacketContents interface from ESATUtil and fills one
telemetry packet.  They are used by ESAT_Wifi.


# ESAT_WifiConnectionStateTelemetry

Fill the ESAT_CONNECTION_STATE (0x00) telemetry packet.


# ESAT_WifiWLANStatusTelemetry

Fill the ESAT_WIFI_WLAN_STATUS (0x01) telemetry packet.


# ESAT_WifiWLANConfigurationTelemetry

Fill the ESAT_WIFI_WLAN_CONFIGURATION (0x02) telemetry packet.


# ESAT_WifiNetworkAndTransportConfigurationTelemetry

Fill the ESAT_WIFI_NETWORK_AND_TRANSPORT_CONFIGURATION (0x03) telemetry packet.


# ESAT_WifiHostnameTelemetry

Fill the ESAT_WIFI_HOSTNAME (0x04) telemetry packet.

