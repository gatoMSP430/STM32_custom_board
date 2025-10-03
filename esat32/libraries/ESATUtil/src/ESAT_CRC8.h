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

#ifndef ESAT_CRC8_H
#define ESAT_CRC8_H

#include <Arduino.h>
#include <Stream.h>

// 8-bit cyclic redundancy check (CRC) calculator with Stream interface.
// Write your message to the CRC calculator and then read the CRC
// remainder (either resetting the CRC calculation with read() or not
// resetting it with peek()).
class ESAT_CRC8: public Printable, public Stream
{
  public:
    // Create a CRC calculator stream with generator polynomial
    // represented by the given byte.
    // The nth bit (the 0th bit being the least significant one)
    // corresponds to the nth coefficient of the polynomial, with
    // an implicit 8th bit set to 1.  For example, the byte
    // B00000111 represents the polynomial x^8 + x^2 + x + 1.
    ESAT_CRC8(byte polynomial);

    // Return 1 if the CRC remainder is available
    // (you wrote data to this CRC stream since the last flush/reset);
    // otherwise return 0.
    int available();

    // Reset the CRC computation.
    void flush();

    // Return the CRC remainder if it is available
    // (you wrote data to this CRC stream since the last flush/reset);
    // otherwise return -1.
    int peek();

    // Print the polynomial of this CRC stream in human-readable form
    // to an output stream.
    size_t printTo(Print& output) const;

    // Return the CRC remainder if it is available
    // (you wrote data to this CRC stream since the last flush/reset);
    // otherwise return -1.
    // Reset the CRC computation.
    int read();

    // Update the CRC remainder with a new byte datum.
    // Return 1.
    size_t write(uint8_t datum);

    // Import size_t Print::write(const uint8_t* buffer, size_t bufferLength).
    // Update the CRC remainder with the given message buffer.
    // Return the number of bytes written.
    using Print::write;

  private:
    // Byte representation of the generator polynomial.
    // The nth bit (the 0th bit being the least significant one)
    // corresponds to the nth coefficient of the polynomial, with
    // an implicit 8th bit set to 1.  For example, the byte
    // B00000111 represents the polynomial x^8 + x^2 + x + 1.
    byte polynomial;

    // Current CRC remainder.
    int remainder = -1;
};

#endif /* ESAT_CRC8 */

