
/*
 * ESAT OBC Main Program version 4.8.0
 * Copyright (C) 2017, 2018, 2019, 2020, 2021 Theia Space, Universidad Politécnica
 * de Madrid.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Theia Space's ESAT OBC library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
 
//#ifndef ARDUINO_ESAT_OBC
//#error Wrong board: you need to use the ESAT-OBC board with this program.
//#endif
#include <ESAT_OnBoardDataHandling.h>
// Main program of the on-board computer.  It performs some initial
// peripheral setup and bookkeeping in setup() and then it runs the
// main on-board data handling loop.

// The on-board data handling (OnBoardDataHandling) module operates on
// ESATSubsystem objects which are a common interface to subsystem
// functionality from the point of view of the on-board data handling
// function.

// Example subsystem.  Use it as the skeleton of your own subsystem.

// Cycle period in milliseconds.
const word PERIOD = 1000;

// Maximum packet data length we will handle.
const word PACKET_DATA_BUFFER_LENGTH = 256;

// Maximum whole packet length we will handle.
const word WHOLE_PACKET_BUFFER_LENGTH =
  ESAT_CCSDSPrimaryHeader::LENGTH + PACKET_DATA_BUFFER_LENGTH;

// Accumulate incoming USB telecommands in this buffer.
byte usbTelecommandBuffer[WHOLE_PACKET_BUFFER_LENGTH];

// Accumulate incoming Wifi telecommands in this buffer.
byte wifiReaderBuffer[WHOLE_PACKET_BUFFER_LENGTH];

// Accumulate the packet data field of a full packet coming
// from the Wifi board in this buffer.
byte wifiPacketDataBuffer[PACKET_DATA_BUFFER_LENGTH];

//SdFatFs fatFs;

// Start peripherals and do the initial bookkeeping here:
// - Activate the reception of telecommands from the USB interface.
// - Activate the emission of telemetry through the USB interface.
// - Register the available subsystems for use by the on-board data
//   handling module.
// - Begin the subsystems.
// - Begin the timer that keeps a precise timing of the main loop.
// This is the first function of the program to be run at it runs only
// once.
void setup()
{
  //RAMStatistics.begin();
  Serial.begin(9600);
  SerialWifi.begin(9600);
  Wire.begin();
  
 while(!SD.begin(SD_DETECT_NONE)) {
      Serial.println("initialization failed. Is a card inserted?");
    delay(10);
  }

  Serial.println("A card is present.");

  ESAT_I2CMaster.begin(Wire);
  delay(1000);
  ESAT_OBCSubsystem.begin();
  ESAT_EPSSubsystem.begin();
  ESAT_ADCSSubsystem.begin();
  ESAT_COMSubsystem.begin();
  ESAT_WifiSubsystem.begin(wifiReaderBuffer,
                           sizeof(wifiReaderBuffer),
                           wifiPacketDataBuffer,
                           sizeof(wifiPacketDataBuffer));
  ESAT_OnBoardDataHandling.enableUSBTelecommands(usbTelecommandBuffer,
                                                 sizeof(usbTelecommandBuffer));
  ESAT_OnBoardDataHandling.enableUSBTelemetry();
  ESAT_OnBoardDataHandling.registerSubsystem(ESAT_OBCSubsystem);
  ESAT_OnBoardDataHandling.registerSubsystem(ESAT_EPSSubsystem);
  ESAT_OnBoardDataHandling.registerSubsystem(ESAT_ADCSSubsystem);
  ESAT_OnBoardDataHandling.registerSubsystem(ESAT_COMSubsystem);
  ESAT_OnBoardDataHandling.registerSubsystem(ESAT_WifiSubsystem);
  if (ESAT_ThermalPayloadSubsystem.enabled)
  {
    ESAT_ThermalPayloadSubsystem.begin();
    ESAT_OnBoardDataHandling.registerSubsystem(ESAT_ThermalPayloadSubsystem);
  }
  ESAT_Timer.begin(PERIOD);
}

// Body of the main loop of the program:
// - Retrieve the incoming telecommands.
// - Dispatch the incoming telecommands on their target subsystems.
// - Update the subsystems.
// - Retrieve the telemetry packets from the subsystems.
// - Forward the retrieved telemetry packets to the subsystems so that
//   they can use them (for example, a subsystem may send telemetry
//   packets to the ground station or it can store them for later use).
// This function is run in an infinite loop that starts after setup().
void loop()
{
  ESAT_Timer.waitUntilNextCycle();
  byte buffer[PACKET_DATA_BUFFER_LENGTH];
  ESAT_CCSDSPacket packet(buffer, sizeof(buffer));
  while (ESAT_OnBoardDataHandling.readTelecommand(packet))
  {
    ESAT_OnBoardDataHandling.dispatchTelecommand(packet);
  }
  ESAT_OnBoardDataHandling.updateSubsystems();
  while (ESAT_OnBoardDataHandling.readSubsystemsTelemetry(packet))
  {
    ESAT_OnBoardDataHandling.writeTelemetry(packet);
  }
}
