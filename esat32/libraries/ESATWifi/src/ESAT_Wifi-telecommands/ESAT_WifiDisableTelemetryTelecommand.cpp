/*
 * Copyright (C) 2018, 2020, 2021 Theia Space, Universidad Politécnica de Madrid
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

#include "ESAT_Wifi-telecommands/ESAT_WifiDisableTelemetryTelecommand.h"
#include "ESAT_Wifi.h"
#include "ESAT_Wifi-hardware/ESAT_WifiConfiguration.h"

boolean ESAT_WifiDisableTelemetryTelecommandClass::handleUserData(ESAT_CCSDSPacket packet)
{
  const byte identifier = packet.readByte();
  if (packet.triedToReadBeyondLength())
  {
    (void) identifier; // Unused.
    return false;
  }
  else
  {
    ESAT_Wifi.disableTelemetry(identifier);
    ESAT_WifiConfiguration.enabledTelemetry.clear(identifier);
    ESAT_WifiConfiguration.writeEnabledTelemetry();
    return true;
  }
}

ESAT_WifiDisableTelemetryTelecommandClass ESAT_WifiDisableTelemetryTelecommand;
