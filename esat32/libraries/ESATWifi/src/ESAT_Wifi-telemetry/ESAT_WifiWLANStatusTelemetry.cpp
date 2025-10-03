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
#include "ESAT_Wifi-telemetry/ESAT_WifiWLANStatusTelemetry.h"
#include "ESAT_Wifi-hardware/ESAT_WifiRadio.h"
#include "ESAT_Wifi-hardware/ESAT_WifiConfiguration.h"

boolean ESAT_WifiWLANStatusTelemetryClass::available()
{
  return true;
}

boolean ESAT_WifiWLANStatusTelemetryClass::fillUserData(ESAT_CCSDSPacket& packet)
{
  writeWLANState(packet);
  writeChannel(packet);
  writeRSSI(packet);
  return true;
}

void ESAT_WifiWLANStatusTelemetryClass::writeChannel(ESAT_CCSDSPacket& packet)
{
  packet.writeByte((byte) WiFi.channel());
}

void ESAT_WifiWLANStatusTelemetryClass::writeRSSI(ESAT_CCSDSPacket& packet)
{
  packet.writeChar((signed char) WiFi.RSSI());
}

void ESAT_WifiWLANStatusTelemetryClass::writeWLANState(ESAT_CCSDSPacket& packet)
{
  switch(wifi_station_get_connect_status())
  {
    case STATION_WRONG_PASSWORD: // Wrong password.
      packet.writeByte(1);
      return;
      break;
    case STATION_NO_AP_FOUND: // No access point.
      packet.writeByte(2);
      return;
      break;
    case STATION_CONNECT_FAIL: // Other error.
      packet.writeByte(3);
      return;
      break;
    case STATION_CONNECTING:
    case STATION_GOT_IP:
    case STATION_IDLE:
    default:
      packet.writeByte(0); // No error.
      return;
      break;
  }
}

ESAT_WifiWLANStatusTelemetryClass ESAT_WifiWLANStatusTelemetry;
