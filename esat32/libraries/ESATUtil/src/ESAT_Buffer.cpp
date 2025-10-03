/*
 * Copyright (C) 2017, 2018, 2019 Theia Space, Universidad Politécnica de Madrid
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

#include "ESAT_Buffer.h"
#include "ESAT_Util.h"

ESAT_Buffer::ESAT_Buffer()
{
  buffer = nullptr;
  bufferCapacity = 0;
  bytesInBuffer = 0;
  readWritePosition = 0;
  references = nullptr;
  triedToReadBeyondBufferLength = false;
  triedToWriteBeyondBufferCapacity = false;
  // Set the timeout for waiting for stream data to zero, as it
  // doesn't make sense to wait when reading from these buffers.
  setTimeout(0);
}

ESAT_Buffer::ESAT_Buffer(const unsigned long capacity)
{
  buffer = new byte[capacity];
  bufferCapacity = capacity;
  bytesInBuffer = 0;
  readWritePosition = 0;
  references = new unsigned long(1);
  triedToReadBeyondBufferLength = false;
  triedToWriteBeyondBufferCapacity = false;
  // Set the timeout for waiting for stream data to zero, as it
  // doesn't make sense to wait when reading from these buffers.
  setTimeout(0);
}

ESAT_Buffer::ESAT_Buffer(byte array[],
                         const unsigned long capacity,
                         const unsigned long availableBytes)
{
  buffer = array;
  bufferCapacity = capacity;
  bytesInBuffer = min(capacity, availableBytes);
  readWritePosition = 0;
  references = nullptr;
  triedToReadBeyondBufferLength = false;
  triedToWriteBeyondBufferCapacity = false;
  // Set the timeout for waiting for stream data to zero, as it
  // doesn't make sense to wait when reading from these buffers.
  setTimeout(0);
}

ESAT_Buffer::ESAT_Buffer(const ESAT_Buffer& original)
{
  buffer = original.buffer;
  bufferCapacity = original.bufferCapacity;
  bytesInBuffer = original.bytesInBuffer;
  dynamicallyAllocated = original.dynamicallyAllocated;
  readWritePosition = original.readWritePosition;
  references = original.references;
  triedToReadBeyondBufferLength = original.triedToReadBeyondBufferLength;
  triedToWriteBeyondBufferCapacity = original.triedToWriteBeyondBufferCapacity;
  _timeout = original._timeout;
  addReference();
}

ESAT_Buffer::~ESAT_Buffer()
{
  removeReference();
}

void ESAT_Buffer::addReference()
{
  if (references != nullptr)
  {
    *references = *references + 1;
  }
}

int ESAT_Buffer::available()
{
  // Truncate the result of availableBytes() to fit a 16-bit signed
  // integer.
  return min(availableBytes(), (unsigned long) 0x7FFF);
}

unsigned long ESAT_Buffer::availableBytes() const
{
  if (readWritePosition < bytesInBuffer)
  {
    return bytesInBuffer - readWritePosition;
  }
  else
  {
    return 0;
  }
}

unsigned long ESAT_Buffer::capacity() const
{
  return bufferCapacity;
}

void ESAT_Buffer::flush()
{
  // Reset the buffer length and rewind.
  bytesInBuffer = 0;
  readWritePosition = 0;
}

unsigned long ESAT_Buffer::length() const
{
  return bytesInBuffer;
}

int ESAT_Buffer::peek()
{
  // Peeking past the last available byte returns -1.
  if (availableBytes() == 0)
  {
    triedToReadBeyondBufferLength = true;
    return -1;
  }
  triedToReadBeyondBufferLength = false;
  return buffer[readWritePosition];
}

unsigned long ESAT_Buffer::position() const
{
  return readWritePosition;
}

size_t ESAT_Buffer::printTo(Print& output) const
{
  // Fall through when the buffer is empty.
  if (bytesInBuffer == 0)
  {
    return 0;
  }
  // Normal operation: print the contents of the buffer.
  size_t bytesWritten =
    output.print(F("0x"));
  bytesWritten =
    bytesWritten
    + output.print(ESAT_Util.byteToHexadecimal(buffer[0]));
  for (unsigned long i = 1; i < bytesInBuffer; i++)
  {
    bytesWritten =
      bytesWritten
      + output.print(F(", 0x"));
    bytesWritten =
      bytesWritten
      + output.print(ESAT_Util.byteToHexadecimal(buffer[i]));
  }
  return bytesWritten;
}

int ESAT_Buffer::read()
{
  const int datum = peek();
  // Advance the read/write position only when there is a byte
  // available.
  if (datum > -1)
  {
    readWritePosition = readWritePosition + 1;
  }
  return datum;
}

boolean ESAT_Buffer::readFrom(Stream& input, const unsigned long bytesToRead)
{
  // As we flush the buffer first, we will lose the original state of
  // the buffer even if the read fails.
  flush();
  // Just fail if we cannot fit the required number of bytes into the
  // buffer.
  if (bytesToRead > bufferCapacity)
  {
    return false;
  }
  // Fall through when the required number of bytes is 0.
  if (bytesToRead == 0)
  {
    return true;
  }
  // Normal operation: read the required bytes from the input stream.
  bytesInBuffer = input.readBytes((char*) buffer, bytesToRead);
  if (bytesInBuffer == bytesToRead)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void ESAT_Buffer::removeReference()
{
  if (references != nullptr)
  {
    *references = *references - 1;
    if (*references == 0)
    {
      delete[] buffer;
      delete references;
    }
  }
}

void ESAT_Buffer::rewind()
{
  readWritePosition = 0;
}

boolean ESAT_Buffer::seek(const unsigned long newPosition)
{
  if (newPosition <= bytesInBuffer)
  {
    readWritePosition = newPosition;
    return true;
  }
  else
  {
    return false;
  }
}

boolean ESAT_Buffer::setLength(const unsigned long newLength)
{
  if (newLength <= bufferCapacity)
  {
    bytesInBuffer = newLength;
    return true;
  }
  else
  {
    return false;
  }
}

boolean ESAT_Buffer::triedToReadBeyondLength() const
{
  return triedToReadBeyondBufferLength;
}

boolean ESAT_Buffer::triedToWriteBeyondCapacity() const
{
  return triedToWriteBeyondBufferCapacity;
}

size_t ESAT_Buffer::write(const uint8_t datum)
{
  // Just fail if we have no backend buffer.
  if (!buffer)
  {
    triedToWriteBeyondBufferCapacity = true;
    return 0;
  }
  // Just fail if we are above capacity.
  if (readWritePosition >= bufferCapacity)
  {
    triedToWriteBeyondBufferCapacity = true;
    return 0;
  }
  // Normal operation: write the datum to the current read/write
  // position of the backend buffer, increment the counters and return
  // the number of written bytes (1).
  triedToWriteBeyondBufferCapacity = false;
  buffer[readWritePosition] = datum;
  readWritePosition = readWritePosition + 1;
  bytesInBuffer = readWritePosition;
  return 1;
}

boolean ESAT_Buffer::writeTo(Stream& output) const
{
  // Just fail if we have no backend buffer.
  if (!buffer)
  {
//	  Serial.println("\n no buffer");
    return false;
  }
  // Avoid writing if the buffer is empty.
  if (bytesInBuffer == 0)
  {
    return true;
  }
  // Normal operation: dump the contents of the buffer.
  const size_t bytesWritten = output.write(buffer, bytesInBuffer);
  //Serial.print("Written: ");Serial.println(bytesWritten);
  //Serial.print("in buffer: ");Serial.println(bytesInBuffer);
  if (bytesWritten < bytesInBuffer)
  {
//	  Serial.println("writing failed");
    return false;
  }
  else
  {
//	  Serial.println("writing ok");
    return true;
  }
}

ESAT_Buffer& ESAT_Buffer::operator=(const ESAT_Buffer& original)
{
  if (this != &original)
  {
    removeReference();
    buffer = original.buffer;
    bufferCapacity = original.bufferCapacity;
    bytesInBuffer = original.bytesInBuffer;
    dynamicallyAllocated = original.dynamicallyAllocated;
    readWritePosition = original.readWritePosition;
    references = original.references;
    triedToReadBeyondBufferLength = original.triedToReadBeyondBufferLength;
    triedToWriteBeyondBufferCapacity = original.triedToWriteBeyondBufferCapacity;
    _timeout = original._timeout;
    addReference();
  }
  return *this;
}
