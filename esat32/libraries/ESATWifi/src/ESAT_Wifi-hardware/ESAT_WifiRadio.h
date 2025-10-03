/*
 * Copyright (C) 2018 Theia Space, Universidad Politécnica de Madrid
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
 * along with Theia Space's ESAT Util library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef ESAT_WifiRadio_h
#define ESAT_WifiRadio_h

#include <Arduino.h>
#include <ESAT_CCSDSPacket.h>
#include <ESAT_CCSDSPacketFromKISSFrameReader.h>
#include <ESP8266WiFi.h>

// Radio interface of the ESAT Wifi board.
// Use the global ESAT_WifiRadio object.
class ESAT_WifiRadioClass
{
  public:
    // Possible states of the connection state machine.
    // State transitions:
    // - From any state to CONNECTING_TO_NETWORK (when commanded to
    // - connect).
    // - From CONNECTING_TO_NETWORK to WAITING_FOR_NETWORK_CONNECTION
    //   (right after configuring the Wifi interface to connect to the
    //   network).
    // - From WAITING_FOR_NETWORK_CONNECTION to CONNECTING_TO_SERVER
    //   (when the network connection is established).
    // - From CONNECTING_TO_SERVER to CONNECTED (when the
    //   client-to-server connection is established).
    // - From CONNECTED to CONNECTING_TO_NETWORK (if the network
    //   connection drops).
    // - From CONNECTED to CONNECTING_TO_SERVER (if the network
    //   connection stays active, but the client-to-server connection
    //   drops).
    // - From any state to DISCONNECTING (when commanded to
    // - disconnect).
    // - From DISCONNECTING to DISCONNECTED (right after disconnecting
    // - from the network).
    enum ConnectionState
    {
      CONNECTING_TO_NETWORK = 0x00,
      WAITING_FOR_NETWORK_CONNECTION = 0x01,
      CONNECTING_TO_SERVER = 0x02,
      CONNECTED = 0x03,
      DISCONNECTING = 0x04,
      DISCONNECTED = 0x05,
    };

    // Use this client to connect to the ground segment server.
    WiFiClient client;

    // Set up the radio.
    // Use the buffer to store incoming packets from call
    // to read() to call to read().
    // Set the timeout for the connection to the network;
    // a network connection attempt will restart if
    // it takes longer than the timeout.
    void begin(byte radioBuffer[],
               unsigned long radioBufferLength,
               byte networkConnectionTimeoutSeconds);

    // Connect to the network and ground segment server.
    // This will take several calls to update().
    void connect();

    // Disconnect from the network and ground segment server.
    // This will take one call to update().
    void disconnect();

    // Return the current connection state.
    ConnectionState readConnectionState() const;

    // Fill the packet with data read from the radio interface.
    // Return true if there was a new packet; otherwise return false.
    boolean read(ESAT_CCSDSPacket& packet);

    // Perform connection management and related tasks.
    void update();

    // Send a packet through the radio interface.
    void write(ESAT_CCSDSPacket packet);

  private:

    // Current state of the connection state machine.
    ConnectionState connectionState;

    // Time when the previous connection attempt started.
    unsigned long networkConnectionStartTimeMilliseconds;

    // Timeout in milliseconds for each network connection attempt.
    // Every time the connection attempt is unsuccessful for longer
    // than the timeout, try connect again.
    unsigned long networkConnectionTimeoutMilliseconds;

    // Use this to read CCSDS packets from KISS frames coming from
    // radio.
    ESAT_CCSDSPacketFromKISSFrameReader radioReader;

    // Connect to the wireless network.
    void connectToNetwork();

    // Connect to the ground segment server.
    void connectToServer();

    // Disconnect from the wireless network and ground station server.
    void disconnectFromNetworkAndServer();

    // Reconnect to the server if disconnected from the server or to
    // the network if disconnected from the network.
    void reconnectIfDisconnected();

    // Check that the network connection is established.
    void waitForNetworkConnection();
};

// Global instance of the Wifi radio library.
extern ESAT_WifiRadioClass ESAT_WifiRadio;

#endif /* ESAT_WifiRadio_h */
