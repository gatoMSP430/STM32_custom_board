/*
 * Copyright (C) 2018 Theia Space, Universidad Politécnica de Madrid
 *
 * This file is part of Theia Space's ESAT Util library.
 *
 * Theia Space's ESAT Util library is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Theia Space's ESAT Util library is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Theia Space's ESAT Util library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "ESAT_CCSDSTelecommandPacketDispatcher.h"

ESAT_CCSDSTelecommandPacketDispatcher::ESAT_CCSDSTelecommandPacketDispatcher(const word theApplicationProcessIdentifier)
{
  applicationProcessIdentifier = theApplicationProcessIdentifier;
  head = nullptr;
}

void ESAT_CCSDSTelecommandPacketDispatcher::add(ESAT_CCSDSTelecommandPacketHandler& handler)
{
  handler.nextTelecommandPacketHandler = head;
  head = &handler;
}

boolean ESAT_CCSDSTelecommandPacketDispatcher::compatiblePacket(ESAT_CCSDSPacket packet) const
{
  const ESAT_CCSDSPrimaryHeader primaryHeader
    = packet.readPrimaryHeader();
  if (primaryHeader.packetType
      != primaryHeader.TELECOMMAND)
  {
    return false;
  }
  if (primaryHeader.secondaryHeaderFlag
      != primaryHeader.SECONDARY_HEADER_IS_PRESENT)
  {
    return false;
  }
  if (primaryHeader.applicationProcessIdentifier
      != applicationProcessIdentifier)
  {
    return false;
  }
  if (packet.length() < ESAT_CCSDSSecondaryHeader::LENGTH)
  {
    return false;
  }
  return true;
}

boolean ESAT_CCSDSTelecommandPacketDispatcher::dispatch(ESAT_CCSDSPacket packet)
{
  packet.rewind();
  if (!compatiblePacket(packet))
  {
    return false;
  }
  ESAT_CCSDSSecondaryHeader secondaryHeader = packet.readSecondaryHeader();
  for (ESAT_CCSDSTelecommandPacketHandler* handler = head;
       handler != nullptr;
       handler = handler->nextTelecommandPacketHandler)
  {
    if (handlerIsCompatibleWithPacket(*handler, secondaryHeader))
    {
      return handler->handleUserData(packet);
    }
  }
  return false;
}

boolean ESAT_CCSDSTelecommandPacketDispatcher::handlerIsCompatibleWithPacket(ESAT_CCSDSTelecommandPacketHandler& handler,
                                                                             ESAT_CCSDSSecondaryHeader secondaryHeader)
{
  const ESAT_SemanticVersionNumber handlerVersionNumber =
    handler.versionNumber();
  if (handlerVersionNumber.isForwardCompatibleWith(secondaryHeader.majorVersionNumber,
                                                   secondaryHeader.minorVersionNumber,
                                                   secondaryHeader.patchVersionNumber)
      && (handler.packetIdentifier() == secondaryHeader.packetIdentifier))
  {
    return true;
  }
  else
  {
    return false;
  }
}
