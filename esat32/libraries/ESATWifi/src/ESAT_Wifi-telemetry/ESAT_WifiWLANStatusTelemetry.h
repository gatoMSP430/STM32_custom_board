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

#ifndef ESAT_WifiWLANStatusTelemetry_h
#define ESAT_WifiWLANStatusTelemetry_h

#include <Arduino.h>
#include <ESAT_CCSDSTelemetryPacketContents.h>

// ESAT Wifi board wireless local area network controller telemetry packet contents.
// ESAT_Wifi uses this.
class ESAT_WifiWLANStatusTelemetryClass: public ESAT_CCSDSTelemetryPacketContents
{
  public:
    // Return true when a new telemetry packet is available; otherwise
    // return false.
    boolean available();

    // Return the packet identifier.
    byte packetIdentifier()
    {
      return 0x01;
    }

    // Fill the user data field of the given packet.
    // The write pointer of the packet is already at the start
    // of the user data field.
    // Return true on success; otherwise return false.
    boolean fillUserData(ESAT_CCSDSPacket& packet);

  private:
    // Write the used Wifi radio channel.
    void writeChannel(ESAT_CCSDSPacket& packet);

    // Write whether or not we are connected to the given packet.
    void writeIsConnected(ESAT_CCSDSPacket& packet);

    // Write the MAC address to the given packet.
    void writeMACAddress(ESAT_CCSDSPacket& packet);

    // Write the received signal strength indicator to the given packet.
    void writeRSSI(ESAT_CCSDSPacket& packet);

    // Write the SSID to the given packet.
    void writeSSID(ESAT_CCSDSPacket& packet);

    // Write the 32 first characters from the given string to the given packet.
    void writeString(String str, ESAT_CCSDSPacket& packet);

    // Write the wlan controller current state to the given packet.
    void writeWLANState(ESAT_CCSDSPacket& packet);
};

// Global instance of ESAT_WifiWLANStatusTelemetry.  ESAT_Wifi uses this
// to fill the ESAT Wifi board connection state telemetry packet.
extern ESAT_WifiWLANStatusTelemetryClass ESAT_WifiWLANStatusTelemetry;

#endif /* ESAT_WifiWLANStatusTelemetry_h */
