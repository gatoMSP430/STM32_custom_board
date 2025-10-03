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
along with Theia Space's ESAT Wifi library.  If not, see
<http://www.gnu.org/licenses/>.


Wifi telecommand handlers provide telecommand responses.  Each
telecommand handler implements the ESAT_CCSDSPacketConsumer interface
from ESATUtil and responds to one or several telecommands.  They are
used by ESAT_Wifi.


# ESAT_WifiConnectTelecommand

Telecommand handler for WIFI_CONNECT (0x00): connect to the network
and ground station server.


# ESAT_WifiDisconnectTelecommand

Telecommand handler for WIFI_DISCONNECT (0x01): disconnect from the
network and ground station server.


# ESAT_WifiSetNetworkSSIDTelecommand

Telecommand handler for WIFI_SET_NETWORK_SSID (0x10): set the network
SSID.


# ESAT_WifiSetNetworkPassphraseTelecommand

Telecommand handler for WIFI_SET_NETWORK_PASSPHRASE (0x11): set the
network passphrase.


# ESAT_WifiSetServerAddressTelecommand

Telecommand handler for WIFI_SET_SERVER_ADDRESS (0x12): set the
server address.


# ESAT_WifiSetServerPortTelecommand

Telecommand handler for WIFI_SET_SERVER_PORT (0x13): set the
server port.


# ESAT_WifiSetHostAddressTelecommand

Telecommand handler for WIFI_SET_HOST_ADDRESS (0x14): set the
static host IP address.


# ESAT_WifiSetSubnetMaskTelecommand

Telecommand handler for WIFI_SET_SUBNET_MASK (0x15): set the
static subnet mask.


# ESAT_WifiSetGatewayAddressTelecommand

Telecommand handler for WIFI_SET_GATEWAY_ADDRESS (0x16): set the
static default gateway IP address.


# ESAT_WifiSetDomainNameSystemServer1AddressTelecommand

Telecommand handler for WIFI_SET_DOMAIN_NAME_SYSTEM_SERVER_1_ADDRESS (0x17): set the
domain name system server 1 IP address.


# ESAT_WifiSetDomainNameSystemServer2AddressTelecommand

Telecommand handler for WIFI_SET_DOMAIN_NAME_SYSTEM_SERVER_2_ADDRESS (0x18): set the
domain name system server 2 IP address.


# ESAT_WifiSetHostnameTelecommand

Telecommand handler for WIFI_SET_HOSTNAME (0x19): set the
hostname.


# ESAT_WifiSetDHCPModeTelecommand

Telecommand handler for  WIFI_SET_DHCP_MODE (0x1A): set the
wifi IP address configuration to dynamic.


# ESAT_WifiSetStaticIPModeTelecommand

Telecommand handler for  WIFI_SET_STATIC_IP_MODE (0x1B): set the
wifi IP address configuration to static (user defined).


# ESAT_WifiReadConfigurationTelecommand

Telecommand handler for WIFI_READ_CONFIGURATION (0x20): load the
wifi settings from non-volatile memory.


# ESAT_WifiWriteConfigurationTelecommand

Telecommand handler for WIFI_WRITE_CONFIGURATION (0x21): save the
wifi settings to non-volatile memory.


# ESAT_WifiSetTimeTelecommand

Telecommand handler for WIFI_SET_TIME (0x30): set the real-time clock.


# ESAT_WifiEnableTelemetryTelecommand

Telecommand handler for WIFI_ENABLE_TELEMETRY (0x40): enable the
generation of a telemetry packet.


# ESAT_WifiDisableTelemetryTelecommand

Telecommand handler for WIFI_DISABLE_TELEMETRY (0x41): disable the
generation of a telemetry packet.


# ESAT_WifiEnableStandaloneModeTelecommand

Telecommand handler for WIFI_ENABLE_STANDALONE_MODE (0x42): enable the
wifi telecommands and telemetry self handling witout OBC.


# ESAT_WifiDisableStandaloneModeTelecommand

Telecommand handler for WIFI_DISABLE_STANDALONE_MODE (0x43): disable the
wifi telecommands and telemetry self handling witout OBC.


# ESAT_WifiConfigureHostnameTelemetryDeliveryTelecommand

Telecommand handler for WIFI_CONFIGURE_HOSTNAME_TELEMETRY_DELIVERY (0x44): set the
delivery mode (continous or under request) of HOSTNAME_TELEMETRY packet.


# ESAT_WifiConfigureWLANConfigurationTelemetryDeliveryTelecommand

Telecommand handler for WIFI_CONFIGURE_WLAN_CONFIGURATION_TELEMETRY_DELIVERY (0x45): set the
delivery mode (continous or under request) of WLAN_CONFIGURATION_TELEMETRY packet.

# ESAT_WifiConfigureNetworkAndTransportConfigurationTelemetryDeliveryTelecommand

Telecommand handler for WIFI_CONFIGURE_NETWORK_AND_TRANSPORT_INFORMATION_TELEMETRY_DELIVERY (0x46): set the
delivery mode (continous or under request) of NETWORK_AND_TRANSPORT_CONFIGURATION_TELEMETRY packet.

