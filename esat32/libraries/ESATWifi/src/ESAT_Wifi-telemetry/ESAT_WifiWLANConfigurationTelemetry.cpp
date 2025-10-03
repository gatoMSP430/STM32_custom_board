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

#include <Arduino.h>
#include "ESAT_Wifi-telemetry/ESAT_WifiWLANConfigurationTelemetry.h"
#include "ESAT_Wifi-hardware/ESAT_WifiRadio.h"
#include "ESAT_Wifi-hardware/ESAT_WifiConfiguration.h"

boolean ESAT_WifiWLANConfigurationTelemetryClass::available()
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

void ESAT_WifiWLANConfigurationTelemetryClass::enablePermanentDelivery()
{
  isPermanentDeliveryEnabled = true;
}

void ESAT_WifiWLANConfigurationTelemetryClass::disablePermanentDelivery()
{
  isPermanentDeliveryEnabled = false;
}

void ESAT_WifiWLANConfigurationTelemetryClass::setRemainingDeliveries(word timesToBeDelivered)
{
  remainingDeliveries = timesToBeDelivered;
  isPermanentDeliveryEnabled = false;
}

void ESAT_WifiWLANConfigurationTelemetryClass::decrementRemainingDeliveries()
{
  if (remainingDeliveries > 0)
  {
    remainingDeliveries = remainingDeliveries - 1;
  }
}

boolean ESAT_WifiWLANConfigurationTelemetryClass::fillUserData(ESAT_CCSDSPacket& packet)
{
  writeSSID(packet);
  writeMACAddress(packet);
  if (isPermanentDeliveryEnabled == false)
  {
    decrementRemainingDeliveries();
  }
  return true;
}

void ESAT_WifiWLANConfigurationTelemetryClass::writeMACAddress(ESAT_CCSDSPacket& packet)
{
  const byte macAddressLength = 6;
  byte mac[macAddressLength];
  WiFi.macAddress(mac);
  for (int index = 0; index < macAddressLength; index = index + 1)
  {
    packet.writeByte(mac[index]);
  }
}

void ESAT_WifiWLANConfigurationTelemetryClass::writeSSID(ESAT_CCSDSPacket& packet)
{
  const String SSIDString = WiFi.SSID();
  const byte length = 32;
  for (int index = 0; index < length; index = index + 1)
  {
    packet.writeChar(SSIDString.charAt(index));
  }
}

ESAT_WifiWLANConfigurationTelemetryClass ESAT_WifiWLANConfigurationTelemetry;
