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

#include "ESAT_CCSDSPacketToKISSFrameWriter.h"
#include "ESAT_KISSStream.h"

ESAT_CCSDSPacketToKISSFrameWriter::ESAT_CCSDSPacketToKISSFrameWriter()
{
  backendStream = nullptr;
}

ESAT_CCSDSPacketToKISSFrameWriter::ESAT_CCSDSPacketToKISSFrameWriter(Stream& backend)
{
  backendStream = &backend;
}

boolean ESAT_CCSDSPacketToKISSFrameWriter::bufferedWrite(ESAT_CCSDSPacket packet)
{
  if (backendStream)
  {
    const unsigned long capacity =
      ESAT_KISSStream::frameLength(packet.length());
    ESAT_KISSStream writer(*backendStream, capacity);
    const size_t beginBytesWritten = writer.beginFrame();
    if (beginBytesWritten < writer.FRAME_BEGIN_LENGTH)
    {
      return false;
    }
    const boolean correctPacketWrite = packet.writeTo(writer);
    if (!correctPacketWrite)
    {
      return false;
    }
    const boolean endBytesWritten = writer.endFrame();
    if (endBytesWritten < writer.FRAME_END_LENGTH)
    {
      return false;
    }
    return true;
  }
  else
  {
    return false;
  }
}

boolean ESAT_CCSDSPacketToKISSFrameWriter::unbufferedWrite(ESAT_CCSDSPacket packet)
{
//	Serial.println("\n ->ESAT_CCSDSPacketToKISSFrameWriter::unbufferedWrite");
  if (backendStream)
  {
//	  Serial.print("\n backendStream->peek(): ");Serial.println(backendStream->peek());
    ESAT_KISSStream writer(*backendStream);
    const size_t beginBytesWritten = writer.beginFrame();
    if (beginBytesWritten < writer.FRAME_BEGIN_LENGTH)
    {
//		Serial.print("\n beginBytesWritten < writer.FRAME_BEGIN_LENGTH : ");
//		Serial.print(beginBytesWritten);Serial.print(" <? ");Serial.println(writer.FRAME_BEGIN_LENGTH);
      return false;
    }
    const boolean correctPacketWrite = packet.writeTo(writer);
    if (!correctPacketWrite)
    {
//		Serial.print("correctPacketWrite: ");Serial.println(correctPacketWrite);
      return false;
    }
    const boolean endBytesWritten = writer.endFrame();
    if (endBytesWritten < writer.FRAME_END_LENGTH)
    {
//		Serial.print("endBytesQritten: ");Serial.print(endBytesWritten);Serial.print(" | ");Serial.println(writer.FRAME_END_LENGTH);
      return false;
    }
//	Serial.println("\n unbufferedWrite success");
    return true;
  }
  else
  {
//	  Serial.println("\n no backendStream");
    return false;
  }
}
