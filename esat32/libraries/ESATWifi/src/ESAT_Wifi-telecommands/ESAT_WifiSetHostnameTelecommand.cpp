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

#include "ESAT_Wifi-telecommands/ESAT_WifiSetHostnameTelecommand.h"
#include "ESAT_Wifi-hardware/ESAT_WifiConfiguration.h"
#include "ESAT_Wifi-hardware/ESAT_WifiRadio.h"

boolean ESAT_WifiSetHostnameTelecommandClass::handleUserData(ESAT_CCSDSPacket packet)
{
  ESAT_Buffer hostname((byte*) ESAT_WifiConfiguration.hostname,
                       sizeof(ESAT_WifiConfiguration.hostname));
  (void) hostname.readFrom(packet, hostname.capacity());
  // Hostname updates doesn't affect WiFi communications.
  // Therefore, hostname is updated as soon as telecommand is received.
  // However, changes won't be stored on non-volatile memory
  // unless "Save settings" were pressed.
  (void) WiFi.hostname(ESAT_WifiConfiguration.hostname);
  return true;
}

ESAT_WifiSetHostnameTelecommandClass ESAT_WifiSetHostnameTelecommand;
