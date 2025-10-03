/*
 * Copyright (C) 2018 Theia Space, Universidad Politécnica de Madrid
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

#include "ESAT_Wifi-telecommands/ESAT_WifiSetServerAddressTelecommand.h"
#include "ESAT_Wifi-hardware/ESAT_WifiConfiguration.h"

boolean ESAT_WifiSetServerAddressTelecommandClass::handleUserData(ESAT_CCSDSPacket packet)
{
  ESAT_Buffer serverAddress((byte*) ESAT_WifiConfiguration.serverAddress,
                            sizeof(ESAT_WifiConfiguration.serverAddress));
  (void) serverAddress.readFrom(packet, serverAddress.capacity());
  return true;
}

ESAT_WifiSetServerAddressTelecommandClass ESAT_WifiSetServerAddressTelecommand;
