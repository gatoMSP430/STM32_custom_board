/*
 * Copyright (C) 2017 Theia Space, Universidad Politécnica de Madrid
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

#include "ESAT_CCSDSPrimaryHeader.h"
#include "ESAT_Buffer.h"

size_t ESAT_CCSDSPrimaryHeader::printTo(Print& output) const
{
  size_t bytesWritten = 0;
  bytesWritten =
    bytesWritten
    + output.println(F("  {"));
  bytesWritten =
    bytesWritten
    + output.print(F("    \"packetVersionNumber\": "));
  bytesWritten =
    bytesWritten
    + output.print(packetVersionNumber, DEC);
  bytesWritten =
    bytesWritten
    + output.println(F(","));
  bytesWritten =
    bytesWritten
    + output.print(F("    \"packetType\": "));
  switch (packetType)
  {
    case TELEMETRY:
      bytesWritten =
        bytesWritten
        + output.print(F("\"TELEMETRY\""));
      break;
    case TELECOMMAND:
      bytesWritten =
        bytesWritten
        + output.print(F("\"TELECOMMAND\""));
      break;
    default:
      bytesWritten =
        bytesWritten
        + output.print(packetType, DEC);
      break;
  }
  bytesWritten =
    bytesWritten
    + output.println(F(","));
  bytesWritten =
    bytesWritten
    + output.print(F("    \"secondaryHeaderFlag\": "));
  switch (secondaryHeaderFlag)
  {
    case SECONDARY_HEADER_IS_NOT_PRESENT:
      bytesWritten =
        bytesWritten
        + output.print(F("\"SECONDARY_HEADER_IS_NOT_PRESENT\""));
      break;
    case SECONDARY_HEADER_IS_PRESENT:
      bytesWritten =
        bytesWritten
        + output.print(F("\"SECONDARY_HEADER_IS_PRESENT\""));
      break;
    default:
      bytesWritten =
        bytesWritten
        + output.print(secondaryHeaderFlag, DEC);
      break;
  }
  bytesWritten =
    bytesWritten
    + output.println(F(","));
  bytesWritten =
    bytesWritten
    + output.print(F("    \"applicationProcessIdentifier\": "));
  bytesWritten =
    bytesWritten
    + output.print(applicationProcessIdentifier, DEC);
  bytesWritten =
    bytesWritten + output.println(F(","));
  bytesWritten =
    bytesWritten + output.print(F("    \"sequenceFlags\": "));
  switch (sequenceFlags)
  {
    case CONTINUATION_SEGMENT_OF_USER_DATA:
      bytesWritten =
        bytesWritten
        + output.print(F("\"CONTINUATION_SEGMENT_OF_USER_DATA\""));
      break;
    case FIRST_SEGMENT_OF_USER_DATA:
      bytesWritten =
        bytesWritten
        + output.print(F("\"FIRST_SEGMENT_OF_USER_DATA\""));
      break;
    case LAST_SEGMENT_OF_USER_DATA:
      bytesWritten =
        bytesWritten
        + output.print(F("\"LAST_SEGMENT_OF_USER_DATA\""));
      break;
    case UNSEGMENTED_USER_DATA:
      bytesWritten =
        bytesWritten
        + output.print(F("\"UNSEGMENTED_USER_DATA\""));
      break;
    default:
      bytesWritten =
        bytesWritten
        + output.print(sequenceFlags, DEC);
      break;
  }
  bytesWritten =
    bytesWritten
    + output.println(F(","));
  bytesWritten =
    bytesWritten
    + output.print(F("    \"packetSequenceCount\": "));
  bytesWritten =
    bytesWritten
    + output.print(packetSequenceCount, DEC);
  bytesWritten =
    bytesWritten
    + output.println(F(","));
  bytesWritten =
    bytesWritten
    + output.print(F("    \"packetDataLength\": "));
  bytesWritten =
    bytesWritten
    + output.print(packetDataLength, DEC);
  bytesWritten =
    bytesWritten
    + output.println(F(""));
  bytesWritten =
    bytesWritten
    + output.print(F("  }"));
  return bytesWritten;
}

boolean ESAT_CCSDSPrimaryHeader::readFrom(Stream& input)
{
  byte octets[LENGTH];
  ESAT_Buffer data(octets, sizeof(octets));
  const boolean correctRead = data.readFrom(input, sizeof(octets));
  if (!correctRead)
  {
    return false;
  }
  const byte firstByte = data.read();
  const byte secondByte = data.read();
  const word firstWord = word(firstByte, secondByte);
  const byte thirdByte = data.read();
  const byte fourthByte = data.read();
  const word secondWord = word(thirdByte, fourthByte);
  const byte fifthByte = data.read();
  const byte sixthByte = data.read();
  const word thirdWord = word(fifthByte, sixthByte);
  packetVersionNumber =
    (firstWord & PACKET_VERSION_NUMBER_MASK)
    >> PACKET_VERSION_NUMBER_OFFSET;
  packetType =
    PacketType((firstWord & PACKET_TYPE_MASK)
               >> PACKET_TYPE_OFFSET);
  secondaryHeaderFlag =
    SecondaryHeaderFlag((firstWord & SECONDARY_HEADER_FLAG_MASK)
                        >> SECONDARY_HEADER_FLAG_OFFSET);
  applicationProcessIdentifier =
    (firstWord & APPLICATION_PROCESS_IDENTIFIER_MASK)
    >> APPLICATION_PROCESS_IDENTIFIER_OFFSET;
  sequenceFlags =
    SequenceFlags((secondWord & SEQUENCE_FLAGS_MASK)
                  >> SEQUENCE_FLAGS_OFFSET);
  packetSequenceCount =
    (secondWord & PACKET_SEQUENCE_COUNT_MASK)
    >> PACKET_SEQUENCE_COUNT_OFFSET;
  packetDataLength =
    ((unsigned long) ((thirdWord & PACKET_DATA_LENGTH_MASK)
                      >> PACKET_DATA_LENGTH_OFFSET))
    + PACKET_DATA_LENGTH_INCREMENT;
  return true;
}

boolean ESAT_CCSDSPrimaryHeader::writeTo(Stream& output) const
{
  const word firstWord =
    ((packetVersionNumber << PACKET_VERSION_NUMBER_OFFSET)
     & PACKET_VERSION_NUMBER_MASK)
    | ((packetType << PACKET_TYPE_OFFSET)
       & PACKET_TYPE_MASK)
    | ((secondaryHeaderFlag << SECONDARY_HEADER_FLAG_OFFSET)
       & SECONDARY_HEADER_FLAG_MASK)
    | ((applicationProcessIdentifier << APPLICATION_PROCESS_IDENTIFIER_OFFSET)
      & APPLICATION_PROCESS_IDENTIFIER_MASK);
  const word secondWord =
    ((sequenceFlags << SEQUENCE_FLAGS_OFFSET)
     & SEQUENCE_FLAGS_MASK)
    | ((packetSequenceCount << PACKET_SEQUENCE_COUNT_OFFSET)
       & PACKET_SEQUENCE_COUNT_MASK);
  const word thirdWord =
    ((packetDataLength - PACKET_DATA_LENGTH_INCREMENT)
     << PACKET_DATA_LENGTH_OFFSET)
    & PACKET_DATA_LENGTH_MASK;
  byte octets[LENGTH];
  ESAT_Buffer data(octets, sizeof(octets));
  (void) data.write(highByte(firstWord));
  (void) data.write(lowByte(firstWord));
  (void) data.write(highByte(secondWord));
  (void) data.write(lowByte(secondWord));
  (void) data.write(highByte(thirdWord));
  (void) data.write(lowByte(thirdWord));
  if (data.length() != LENGTH)
  {
    return false;
  }
  return data.writeTo(output);
}
