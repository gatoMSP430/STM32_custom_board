/*
 * Copyright (C) 2019, 2021 Theia Space, Universidad Politécnica de Madrid
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

#include "ESAT_Wifi-hardware/ESAT_WifiRadio.h"
#include "ESAT_Wifi-hardware/ESAT_WifiConfiguration.h"
#include <ESAT_CCSDSPacketToKISSFrameWriter.h>

void ESAT_WifiRadioClass::begin(byte radioBuffer[],
                                unsigned long radioBufferLength,
                                const byte networkConnectionTimeoutSeconds)
{
  connectionState = DISCONNECTED;
  networkConnectionTimeoutMilliseconds =
    1000 * ((unsigned long) networkConnectionTimeoutSeconds);
  // We pass around packets in KISS frames.
  radioReader = ESAT_CCSDSPacketFromKISSFrameReader(client,
                                                    radioBuffer,
                                                    radioBufferLength);
}

void ESAT_WifiRadioClass::connect()
{
  connectionState = CONNECTING_TO_NETWORK;
}

void ESAT_WifiRadioClass::connectToNetwork()
{
  disconnectFromNetworkAndServer();
  // If static mode is set, IP seetings are required to be configured
  // previously to begin().  Otherwise DHCP will be selected.
  if (ESAT_WifiConfiguration.hostConfigurationMode ==
      ESAT_WifiConfigurationClass::STATIC_HOST_CONFIGURATION_MODE)
  {
    IPAddress ip((uint8_t*) ESAT_WifiConfiguration.hostAddress);
    IPAddress mask((uint8_t*) ESAT_WifiConfiguration.subnetMask);
    IPAddress gateway((uint8_t*) ESAT_WifiConfiguration.gatewayAddress);
    IPAddress dns1((uint8_t*) ESAT_WifiConfiguration.domainNameSystemServer1Address);
    IPAddress dns2((uint8_t*) ESAT_WifiConfiguration.domainNameSystemServer2Address);
    (void) WiFi.config(ip, gateway, mask, dns1, dns2);
  }
  else
  {
    (void) WiFi.config(0, 0, 0);
  }
  (void) WiFi.begin(ESAT_WifiConfiguration.networkSSID,
                    ESAT_WifiConfiguration.networkPassphrase);
  (void) WiFi.hostname(ESAT_WifiConfiguration.hostname);
  connectionState = WAITING_FOR_NETWORK_CONNECTION;
  networkConnectionStartTimeMilliseconds = millis();
}

void ESAT_WifiRadioClass::connectToServer()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    if (client.connect(ESAT_WifiConfiguration.serverAddress,
                       ESAT_WifiConfiguration.serverPort))
    {
      connectionState = CONNECTED;
    }
    else
    {
      connectionState = CONNECTING_TO_SERVER;
    }
  }
  else
  {
    connectionState = CONNECTING_TO_NETWORK;
  }
}

void ESAT_WifiRadioClass::disconnect()
{
  connectionState = DISCONNECTING;
}

void ESAT_WifiRadioClass::disconnectFromNetworkAndServer()
{
  if (client.connected())
  {
    client.stop();
  }
  (void) WiFi.disconnect(true);
  connectionState = DISCONNECTED;
}

ESAT_WifiRadioClass::ConnectionState ESAT_WifiRadioClass::readConnectionState() const
{
  return connectionState;
}

boolean ESAT_WifiRadioClass::read(ESAT_CCSDSPacket& packet)
{
  if (connectionState != CONNECTED)
  {
    return false;
  }
  return radioReader.read(packet);
}

void ESAT_WifiRadioClass::reconnectIfDisconnected()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    connectionState = CONNECTING_TO_NETWORK;
    return;
  }
  if (!client.connected())
  {
    connectionState = CONNECTING_TO_SERVER;
    return;
  }
  connectionState = CONNECTED;
}

void ESAT_WifiRadioClass::update()
{
  switch (connectionState)
  {
    case CONNECTING_TO_NETWORK:
      connectToNetwork();
      break;
    case WAITING_FOR_NETWORK_CONNECTION:
      waitForNetworkConnection();
      break;
    case CONNECTING_TO_SERVER:
      connectToServer();
      break;
    case CONNECTED:
      reconnectIfDisconnected();
      break;
    case DISCONNECTING:
      disconnectFromNetworkAndServer();
      break;
    case DISCONNECTED:
      break;
    default:
      break;
  }
}

void ESAT_WifiRadioClass::waitForNetworkConnection()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    connectionState = CONNECTING_TO_SERVER;
  }
  else
  {
    connectionState = WAITING_FOR_NETWORK_CONNECTION;
    const unsigned long currentTime = millis();
    const unsigned long ellapsedTime =
      currentTime - networkConnectionStartTimeMilliseconds;
    if (ellapsedTime > networkConnectionTimeoutMilliseconds)
    {
      connectionState = CONNECTING_TO_NETWORK;
    }
  }
}

void ESAT_WifiRadioClass::write(ESAT_CCSDSPacket packet)
{
  if (connectionState == CONNECTED)
  {
    // We pass around packets in KISS frames.
    ESAT_CCSDSPacketToKISSFrameWriter radioWriter(client);
    (void) radioWriter.bufferedWrite(packet);
  }
}

ESAT_WifiRadioClass ESAT_WifiRadio;
