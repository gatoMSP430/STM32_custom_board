/*
 * Copyright (C) 2017, 2018, 2019, 2020, 2021 Theia Space, Universidad Politécnica de Madrid
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

#ifndef ESAT_Wifi_h
#define ESAT_Wifi_h

#include <Arduino.h>
#include <ESAT_CCSDSPacket.h>
#include <ESAT_CCSDSTelemetryPacketContents.h>
#include <ESAT_CCSDSPacketFromKISSFrameReader.h>
#include <ESAT_CCSDSTelecommandPacketDispatcher.h>
#include <ESAT_CCSDSTelecommandPacketHandler.h>
#include <ESAT_CCSDSTelemetryPacketBuilder.h>
#include <ESAT_FlagContainer.h>
#include <ESAT_SoftwareClock.h>
#include <ESP8266WiFi.h>

// Wifi board module.
// The Wifi board goes into a socket in the OBC board and has the
// following responsibilities:
// - Connect through the wireless interface to a ground segment
//   server.
// - Connect trhough the serial interface to the OBC.
// - Receive telecommand packets from the ground segment server and
//   forward them to the OBC.
// - Receive telecommand packets from the OBC and execute them.
// - Receive telemetry packets from the OBC and forward them to the
//   ground segment server.
// The telecommands handled by the Wifi board are for simple network
// connection configuration actions.
// Use the global ESAT_Wifi object to access the functionality of
// the Wifi board.  See the example program.
class ESAT_WifiClass
{
  public:
    // Register a telecommand handler.
    void addTelecommand(ESAT_CCSDSTelecommandPacketHandler& telecommand);

    // Register a telemetry packet.
    // The telemetry packet will be enabled or disabled depending on
    // the stored configuration; it can be enabled with
    // enableTelemetry() and disabled with disableTelemetry().
    void addTelemetry(ESAT_CCSDSTelemetryPacketContents& telemetry);

    // Check if wifi telecommands are self processed or not.
    boolean areWifiRadioTelecommandsSelfProcessingEnabled();

    // Set up the Wifi board.
    // Use the radio buffer to store packets coming from the
    // radio/wifi interface.
    // Use the serial buffer to store packets coming from the
    // serial interface.
    // When trying to connect to the network, restart the process
    // if it takes longer that the network connection timeout.
    void begin(byte radioBuffer[],
               unsigned long radioBufferLength,
               byte serialBuffer[],
               unsigned long serialBufferLength,
               byte networkConnectionTimeoutSeconds);

    // Disable the processing of wifi telecommands received by wifi
    // radio without being passed to OBC before.
    void disableSelfProcessingWifiTelecommands();

    // Disable the generation of the telemetry packet with the given
    // identifier.
    void disableTelemetry(byte identifier);

    // Disable the delivery of wifi telemetry by wifi radio without
    // relying on OBC.
    void disableWifiTelemetryRadioDelivery();

    // Enable the processing of wifi telecommands received by wifi
    // radio witout being passed to OBC before.
    void enableSelfProcessingWifiTelecommands();

    // Enable the generation of the telemetry packet with the given
    // identifier.
    void enableTelemetry(byte identifier);

    // Enable the delivery of wifi telemetry by wifi radio without
    // relying on OBC.
    void enableWifiTelemetryRadioDelivery();

    // Handle a telecommand.
    void handleTelecommand(ESAT_CCSDSPacket& packet);

    // Check if the packet is a telecommand for the wifi module.
    boolean isWifiTelecommand(ESAT_CCSDSPacket& packet);

    // Check if wifi telemetry is also delivered by radio.
    boolean isWifiTelemetryRadioDeliveryEnabled();

    // Fill the packet with data read from the radio interface.
    // Return true if there was a new packet; otherwise return false.
    boolean readPacketFromRadio(ESAT_CCSDSPacket& packet);

    // Fill the packet with data read from the serial interface.
    // Return true if there was a new packet; otherwise return false.
    boolean readPacketFromSerial(ESAT_CCSDSPacket& packet);

    // Fill a new ESAT Wifi board telemetry packet.
    // Return true if there was a new packet; otherwise return false.
    boolean readTelemetry(ESAT_CCSDSPacket& packet);

    // Set the time of the real-time clock.
    void setTime(ESAT_Timestamp timestamp);

    // Perform connection management and related tasks.
    void update();

    // Send a packet through the radio interface.
    void writePacketToRadio(ESAT_CCSDSPacket& packet);

    // Send a packet through the serial interface.
    void writePacketToSerial(ESAT_CCSDSPacket& packet);

  private:
    // Unique identifier of the Wifi board for telemetry and
    // telecommand purposes.
    static const word APPLICATION_PROCESS_IDENTIFIER = 3;

    // Version numbers.
    static const byte MAJOR_VERSION_NUMBER = 2;
    static const byte MINOR_VERSION_NUMBER = 2;
    static const byte PATCH_VERSION_NUMBER = 0;

    // Line for signaling that the Wifi board is not connected to the server.
    static const byte NOT_CONNECTED_SIGNAL_PIN = 0;

    // Line for signaling a telemetry queue reset condition.
    static const byte RESET_TELEMETRY_QUEUE_PIN = 2;

    // Flag for enabling standalone wifi telecommand processing.
    boolean areWifiRadioTelecommandsSelfProcessed;

    // Flag for enabling wifi telemetry wifi radio delivery.
    boolean isTelemetryEnabledOnWifiRadio;

    // Use this clock for timekeeping.
    ESAT_SoftwareClock clock;

    // True when the telemetry queue must be reset; false otherwise.
    // This variable is volatile because it can change in response to
    // interrupts.
    volatile boolean mustResetTelemetryQueue;

    // List of pending telemetry packet identifiers.
    ESAT_FlagContainer pendingTelemetry;

    // Use this to read CCSDS packets from KISS frames coming from
    // serial.
    ESAT_CCSDSPacketFromKISSFrameReader serialReader;

    // Use this for handling telecommand packets.
    ESAT_CCSDSTelecommandPacketDispatcher telecommandPacketDispatcher =
      ESAT_CCSDSTelecommandPacketDispatcher(APPLICATION_PROCESS_IDENTIFIER);

    // Use this for building telemetry packets.
    ESAT_CCSDSTelemetryPacketBuilder telemetryPacketBuilder =
      ESAT_CCSDSTelemetryPacketBuilder(APPLICATION_PROCESS_IDENTIFIER,
                                       MAJOR_VERSION_NUMBER,
                                       MINOR_VERSION_NUMBER,
                                       PATCH_VERSION_NUMBER,
                                       clock);
    // Configure the hardware.
    // Use the radio buffer to store packets coming from the
    // radio/wifi interface.
    // Use the serial buffer to store packets coming from the
    // serial interface.
    // When trying to connect to the network, restart the process
    // if it takes longer that the network connection timeout.
    void beginHardware(byte radioBuffer[],
                       unsigned long radioBufferLength,
                       byte serialBuffer[],
                       unsigned long serialBufferLength,
                       byte networkConnectionTimeoutSeconds);

    // Initialize class variables
    void beginSoftware();

    // Configure the telecommand handlers.
    void beginTelecommands();

    // Configure the telemetry packets.
    void beginTelemetry();

    // Reset the telemetry queue.
    void resetTelemetryQueue();

    // Mark the telemetry queue for reset.
    // This function must always live in RAM,
    // so mark it with ICACHE_RAM_ATTR.
    static void ICACHE_RAM_ATTR signalTelemetryQueueReset();
};

extern ESAT_WifiClass ESAT_Wifi;

#endif /* ESAT_Wifi_h */
