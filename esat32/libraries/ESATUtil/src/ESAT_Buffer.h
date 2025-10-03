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

#ifndef ESAT_Buffer_h
#define ESAT_Buffer_h

#include <Arduino.h>
#include <Stream.h>

// Stream interface to a byte buffer with bounds checking:
// writes and reads will never go beyond the buffer limits.
class ESAT_Buffer: public Printable, public Stream
{
  public:
    // Instantiate an empty buffer.
    // An empty buffer will fail on reads and writes.
    ESAT_Buffer();

    // Instantiate a buffer of given capacity.
    // The buffer will allocate its own memory.
    ESAT_Buffer(unsigned long capacity);

    // Instantiate a buffer backed by a byte array of given capacity.
    // The number of available bytes (with a default value of 0 when
    // not provided) is the number of bytes that can be read as soon
    // as this buffer object is created.  It can't be greater than the
    // capacity of the backend array, so the actual number of
    // available bytes will be truncated to the capacity when the
    // provided value is too great.  Calls to length() will return
    // this number of available bytes and calls to peek() and read()
    // will return useful data as long as position() < capacity.
    ESAT_Buffer(byte array[],
                unsigned long capacity,
                unsigned long availableBytes = 0);

    // Copy constructor.
    // Instantiate a buffer with the same backend memory as another buffer.
    ESAT_Buffer(const ESAT_Buffer& original);

    // Destroy a buffer.
    ~ESAT_Buffer();

    // Return the number of unread bytes available in the buffer,
    // truncated to an int.
    int available();

    // Return the number of unread bytes available in the buffer.
    unsigned long availableBytes() const;

    // Return the capacity of the buffer.
    unsigned long capacity() const;

    // Clear the contents of the buffer.
    // Set the read/write position to the start of the buffer.
    // Set the number of available unread bytes to 0.
    void flush();

    // Return the number of bytes stored in the buffer.
    unsigned long length() const;

    // Return the next byte (or -1 if no byte could be read)
    // without advancing to the next one.
    int peek();

    // Return the read/write position.
    unsigned long position() const;

    // Print the contents of the buffer in human readable form to an
    // output stream.
    size_t printTo(Print& output) const;

    // Return the next byte (or -1 if no byte could be read).
    // Advance the read/write position by 1, bounded by the number
    // of bytes stored in the buffer as returned by length().
    int read();

    // Read a number of bytes from an input stream and fill the
    // buffer from the start with them.
    // Return true on success; otherwise return false.
    // The read/write position will be at the start of the buffer.
    boolean readFrom(Stream& input, unsigned long bytesToRead);

    // Set the read/write position to the start of the buffer.
    void rewind();

    // Set the read/write position to the desired value.
    // Return true on success; otherwise (when the new position
    // is greater than length() and it can't be set) return false.
    boolean seek(unsigned long newPosition);

    // Set the number of stored bytes in the buffer returned by future
    // calls to length() to the requested new length.
    // Return true on success; otherwise (when the new length is
    // greater than capacity() and it can't be set) return false.
    // Calls to peek() and read() return useful data as long as
    // position() < length(), so setLength() changes the number
    // of readable bytes in the buffer.  This is useful, for example,
    // if the contents of the backend array are at some place other
    // than this ESAT_Buffer object.
    boolean setLength(unsigned long newLength);

    // Return true if the last read() or peek() attempt was beyond
    // the length() of the buffer; otherwise return false.
    boolean triedToReadBeyondLength() const;

    // Return true if the last write() attempt was beyond
    // the capacity() of the buffer; otherwise return false.
    boolean triedToWriteBeyondCapacity() const;

    // Write a byte if the read/write position hasn't reached the
    // capacity.
    // Return the actual number of bytes written.
    // Advance the read/write position by 1, bounded by the capacity.
    size_t write(uint8_t datum);

    // Import size_t Print::write(const uint8_t* buffer, size_t bufferLength).
    // Write a byte buffer of given length.
    // Return the number of bytes written.
    using Print::write;

    // Write the contents of the buffer to an output stream.
    // Return true on success; otherwise return false.
    boolean writeTo(Stream& output) const;

    // Assignment operator: make this buffer have the same backend
    // memory as another buffer.
    ESAT_Buffer& operator=(const ESAT_Buffer& original);

  private:
    // Backend buffer.
    byte* buffer;

    // Capacity of the backend buffer.
    unsigned long bufferCapacity;

    // Number of bytes written to the buffer.
    unsigned long bytesInBuffer;

    // True when the backend buffer was allocated dynamically.
    boolean dynamicallyAllocated;

    // Position of the next read or write operation.
    unsigned long readWritePosition;

    // Reference count.
    // Used only when the buffer manages its own memory.
    unsigned long* references;

    // Set to true if the last read() or peek() attempt was beyond
    // the length() of the buffer; otherwise set to false.
    boolean triedToReadBeyondBufferLength;

    // Set to true if the last write() attempt was beyond
    // the capacity() of the buffer; otherwise set to false.
    boolean triedToWriteBeyondBufferCapacity;

    // If the buffer manages its own backend memory, add a reference
    // to the reference count.
    void addReference();

    // If the buffer manages its own backend memory, remove a
    // reference from the reference count.  If the reference count
    // goes to zero, free the backend buffer.
    void removeReference();
};

#endif /* ESAT_Buffer_h */
