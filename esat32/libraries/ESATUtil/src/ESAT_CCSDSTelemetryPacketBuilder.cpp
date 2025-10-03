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

#include "ESAT_CCSDSTelemetryPacketBuilder.h"

ESAT_CCSDSTelemetryPacketBuilder::ESAT_CCSDSTelemetryPacketBuilder()
{
  clock = nullptr;
  head = nullptr;
}

ESAT_CCSDSTelemetryPacketBuilder::ESAT_CCSDSTelemetryPacketBuilder(const word theApplicationProcessIdentifier,
                                                                   const byte theMajorVersionNumber,
                                                                   const byte theMinorVersionNumber,
                                                                   const byte thePatchVersionNumber,
                                                                   ESAT_Clock& theClock)
{
  applicationProcessIdentifier = theApplicationProcessIdentifier;
  majorVersionNumber = theMajorVersionNumber;
  minorVersionNumber = theMinorVersionNumber;
  patchVersionNumber = thePatchVersionNumber;
  clock = &theClock;
  packetSequenceCount = 0;
  head = nullptr;
}

void ESAT_CCSDSTelemetryPacketBuilder::add(ESAT_CCSDSTelemetryPacketContents& newPacketContents)
{
  newPacketContents.nextTelemetryPacketContents = head;
  head = &newPacketContents;
}

ESAT_FlagContainer ESAT_CCSDSTelemetryPacketBuilder::available()
{
  ESAT_FlagContainer result;
  result.clearAll();
  for (ESAT_CCSDSTelemetryPacketContents* contents = head;
       contents != nullptr;
       contents = contents->nextTelemetryPacketContents)
  {
    if (contents->available())
    {
      result.set(contents->packetIdentifier());
    }
  }
  return result;
}

boolean ESAT_CCSDSTelemetryPacketBuilder::build(ESAT_CCSDSPacket& packet,
                                                byte identifier)
{
  if (!clock)
  {
    return false;
  }
  if (packet.capacity() < ESAT_CCSDSSecondaryHeader::LENGTH)
  {
    return false;
  }
  ESAT_CCSDSTelemetryPacketContents* const contents = find(identifier);
  if (contents == nullptr)
  {
    return false;
  }
  packet.writeTelemetryHeaders(applicationProcessIdentifier,
                               packetSequenceCount,
                               clock->read(),
                               majorVersionNumber,
                               minorVersionNumber,
                               patchVersionNumber,
                               identifier);
  const boolean userDataCorrect = contents->fillUserData(packet);
  if (packet.triedToWriteBeyondCapacity())
  {
    return false;
  }
  if (userDataCorrect)
  {
    packetSequenceCount = packetSequenceCount + 1;
    return true;
  }
  else
  {
    return false;
  }
}

ESAT_CCSDSTelemetryPacketContents* ESAT_CCSDSTelemetryPacketBuilder::find(const byte identifier)
{
  for (ESAT_CCSDSTelemetryPacketContents* contents = head;
       contents != nullptr;
       contents = contents->nextTelemetryPacketContents)
  {
    if (contents->packetIdentifier() == identifier)
    {
      return contents;
    }
  }
  // If we didn't find anything, just return nullptr.
  return nullptr;
}
