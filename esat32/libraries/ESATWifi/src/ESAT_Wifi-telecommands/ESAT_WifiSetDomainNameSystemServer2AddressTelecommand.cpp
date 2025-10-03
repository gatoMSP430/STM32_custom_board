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

#include "ESAT_Wifi-telecommands/ESAT_WifiSetDomainNameSystemServer2AddressTelecommand.h"
#include "ESAT_Wifi-hardware/ESAT_WifiConfiguration.h"

boolean ESAT_WifiSetDomainNameSystemServer2AddressTelecommandClass::handleUserData(ESAT_CCSDSPacket packet)
{
  ESAT_Buffer domainNameSystemServer2Address((byte*) ESAT_WifiConfiguration.domainNameSystemServer2Address,
                                             sizeof(ESAT_WifiConfiguration.domainNameSystemServer2Address));
  (void) domainNameSystemServer2Address.readFrom(packet, domainNameSystemServer2Address.capacity());
  return true;
}

ESAT_WifiSetDomainNameSystemServer2AddressTelecommandClass ESAT_WifiSetDomainNameSystemServer2AddressTelecommand;
