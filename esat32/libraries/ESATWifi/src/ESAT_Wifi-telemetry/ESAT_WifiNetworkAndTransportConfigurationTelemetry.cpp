/*
 * Copyright (C) 2019 Theia Space, Universidad Politécnica de Madrid
 *
 * This file is part of Theia Space's ESAT Wifi library.
 *
 * Theia Space's ESAT Wifi library is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Theia Space's ESAT Wifi library is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Theia Space's ESAT Wifi library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "ESAT_Wifi-telemetry/ESAT_WifiNetworkAndTransportConfigurationTelemetry.h"
#include "ESAT_Wifi-hardware/ESAT_WifiRadio.h"
#include "ESAT_Wifi-hardware/ESAT_WifiConfiguration.h"

boolean ESAT_WifiNetworkAndTransportConfigurationTelemetryClass::available()
{
  if (isPermanentDeliveryEnabled == false)
  {
    if (remainingDeliveries <= 0)
    {
      return false;
    }
  }
  return true;
}

void ESAT_WifiNetworkAndTransportConfigurationTelemetryClass::enablePermanentDelivery()
{
  isPermanentDeliveryEnabled = true;
}

void ESAT_WifiNetworkAndTransportConfigurationTelemetryClass::disablePermanentDelivery()
{
  isPermanentDeliveryEnabled = false;
}

void ESAT_WifiNetworkAndTransportConfigurationTelemetryClass::setRemainingDeliveries(word timesToBeDelivered)
{
  remainingDeliveries = timesToBeDelivered;
  isPermanentDeliveryEnabled = false;
}

void ESAT_WifiNetworkAndTransportConfigurationTelemetryClass::decrementRemainingDeliveries()
{
  if (remainingDeliveries > 0)
  {
    remainingDeliveries = remainingDeliveries - 1;
  }
}

boolean ESAT_WifiNetworkAndTransportConfigurationTelemetryClass::fillUserData(ESAT_CCSDSPacket& packet)
{
  writeLocalIPAddress(packet);
  writeSubnetMask(packet);
  writeGatewayIPAddress(packet);
  writeDNSIPAddresses(packet);
  writeIsDHCPEnabled(packet);
  writeServerIPAddress(packet);
  writeServerPort(packet);
  if (isPermanentDeliveryEnabled == false)
  {
    decrementRemainingDeliveries();
  }
  return true;
}

void ESAT_WifiNetworkAndTransportConfigurationTelemetryClass::writeDNSIPAddresses(ESAT_CCSDSPacket& packet)
{
  const byte dnsIPAddresses = 2;
  for (int dns = 0; dns < dnsIPAddresses; dns = dns + 1)
  {
    writeIPAddress(WiFi.dnsIP(dns),
                   packet);
  }
}

void ESAT_WifiNetworkAndTransportConfigurationTelemetryClass::writeGatewayIPAddress(ESAT_CCSDSPacket& packet)
{
  writeIPAddress(WiFi.gatewayIP(),
                 packet);
}

void ESAT_WifiNetworkAndTransportConfigurationTelemetryClass::writeIPAddress(const IPAddress address,
                                                                             ESAT_CCSDSPacket& packet)
{
  const byte ipAddressLength = 4;
  for (int index = 0; index < ipAddressLength; index = index + 1)
  {
    packet.writeByte(address[index]);
  }
}

void ESAT_WifiNetworkAndTransportConfigurationTelemetryClass::writeIsDHCPEnabled(ESAT_CCSDSPacket& packet)
{
  // Low level interface information.
  if (wifi_station_dhcpc_status() == DHCP_STARTED)
  {
    packet.writeByte(0);
  }
  else
  {
    packet.writeByte(1);
  }
}

void ESAT_WifiNetworkAndTransportConfigurationTelemetryClass::writeLocalIPAddress(ESAT_CCSDSPacket& packet)
{
  writeIPAddress(WiFi.localIP(),
                 packet);
}

void ESAT_WifiNetworkAndTransportConfigurationTelemetryClass::writeServerIPAddress(ESAT_CCSDSPacket& packet)
{
  writeIPAddress(ESAT_WifiRadio.client.remoteIP(),
                 packet);
}

void ESAT_WifiNetworkAndTransportConfigurationTelemetryClass::writeServerPort(ESAT_CCSDSPacket& packet)
{
  packet.writeWord((word) ESAT_WifiRadio.client.remotePort());
}

void ESAT_WifiNetworkAndTransportConfigurationTelemetryClass::writeSubnetMask(ESAT_CCSDSPacket& packet)
{
  writeIPAddress(WiFi.subnetMask(),
                 packet);
}

ESAT_WifiNetworkAndTransportConfigurationTelemetryClass ESAT_WifiNetworkAndTransportConfigurationTelemetry;
