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

#ifndef ESAT_WifiNetworkAndTransportConfigurationTelemetry_h
#define ESAT_WifiNetworkAndTransportConfigurationTelemetry_h

#include <Arduino.h>
#include <ESAT_CCSDSTelemetryPacketContents.h>
#include <IPAddress.h>

// ESAT Wifi board TCP/IP parameters telemetry packet contents.
// ESAT_Wifi uses this.
class ESAT_WifiNetworkAndTransportConfigurationTelemetryClass: public ESAT_CCSDSTelemetryPacketContents
{
  public:
    // Return true when a new telemetry packet is available; otherwise
    // return false.
    boolean available();

    // Return the packet identifier.
    byte packetIdentifier()
    {
      return 0x03;
    }

    // Fill the user data field of the given packet.
    // The write pointer of the packet is already at the start
    // of the user data field.
    // Return true on success; otherwise return false.
    boolean fillUserData(ESAT_CCSDSPacket& packet);

    // Enable the continuous delivery of this telemetry.
    void enablePermanentDelivery();

    // Decrease remaining deliveries counter by 1.
    void decrementRemainingDeliveries();

    // Disable the continuous delivery of this telemetry.
    void disablePermanentDelivery();

    // Configure this telemetry to be delivered only for certain times.
    // Also clears permanent delivery flag.
    void setRemainingDeliveries(word timesToBeDelivered);

  private:
    // Enable the continuous delivery of this telemetry.
    boolean isPermanentDeliveryEnabled = false;

    // Count the times this telemetry has to be delivered.
    word remainingDeliveries = 0;

    // Write the DNS IP addresses to the given packet.
    void writeDNSIPAddresses(ESAT_CCSDSPacket& packet);

    // Write the gateway IP address to the given packet.
    void writeGatewayIPAddress(ESAT_CCSDSPacket& packet);

    // Write the given IP address to the given packet.
    void writeIPAddress(IPAddress address, ESAT_CCSDSPacket& packet);

    // Write the DHCP enabled flag to the given packet.
    void writeIsDHCPEnabled(ESAT_CCSDSPacket& packet);

    // Write the local IP address to the given packet.
    void writeLocalIPAddress(ESAT_CCSDSPacket& packet);

    // Write the configured server IP address.
    void writeServerIPAddress(ESAT_CCSDSPacket& packet);

    // Write the configured server connection port.
    void writeServerPort(ESAT_CCSDSPacket& packet);

    // Write the subnet mask to the given packet.
    void writeSubnetMask(ESAT_CCSDSPacket& packet);
};

// Global instance of ESAT_WifiNetworkAndTransportConfigurationTelemetry. ESAT_Wifi uses this
// to fill the ESAT Wifi board network and transport information telemetry packet.
extern ESAT_WifiNetworkAndTransportConfigurationTelemetryClass ESAT_WifiNetworkAndTransportConfigurationTelemetry;

#endif /* ESAT_WifiNetworkAndTransportConfigurationTelemetry_h */
