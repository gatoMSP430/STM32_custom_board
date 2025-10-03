/*
 * Copyright (C) 2019, 2020, 2021 Theia Space, Universidad Politécnica de Madrid
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

#include "ESAT_WifiConfiguration.h"
#include <EEPROM.h>
#include "ESAT_WifiRadio.h"

void ESAT_WifiConfigurationClass::begin()
{
  EEPROM.begin(CONFIGURATION_LENGTH);
}

void ESAT_WifiConfigurationClass::readConfiguration()
{
  readNetworkSSID();
  readNetworkPassphrase();
  readServerAddress();
  readServerPort();
  readHostAddress();
  readSubnetMask();
  readGatewayAddress();
  readDNSServer1Address();
  readDNSServer2Address();
  readHostConfigurationMode();
  readHostname();
  readEnabledTelemetry();
  // Hostname is inocuous, so it is updated automatically.
  (void) WiFi.hostname((char*) hostname);
}

void ESAT_WifiConfigurationClass::readDNSServer1Address()
{
  domainNameSystemServer1Address[0] = EEPROM.read(DNS_1_ADDRESS_OFFSET);
  domainNameSystemServer1Address[1] = EEPROM.read(DNS_1_ADDRESS_OFFSET + 1);
  domainNameSystemServer1Address[2] = EEPROM.read(DNS_1_ADDRESS_OFFSET + 2);
  domainNameSystemServer1Address[3] = EEPROM.read(DNS_1_ADDRESS_OFFSET + 3);
}

void ESAT_WifiConfigurationClass::readDNSServer2Address()
{
  domainNameSystemServer2Address[0]=EEPROM.read(DNS_2_ADDRESS_OFFSET);
  domainNameSystemServer2Address[1]=EEPROM.read(DNS_2_ADDRESS_OFFSET + 1);
  domainNameSystemServer2Address[2]=EEPROM.read(DNS_2_ADDRESS_OFFSET + 2);
  domainNameSystemServer2Address[3]=EEPROM.read(DNS_2_ADDRESS_OFFSET + 3);
}

void ESAT_WifiConfigurationClass::readEnabledTelemetry()
{
  // Read the list of enabled telemetry packets from EEPROM.  Bytes
  // that have never been written have all bits set to 1; if we want
  // never-enabled packets to be disabled by default, we must store
  // enabled packets as 0s and disabled packets as 1s.
  for (int octet = 0; octet < ENABLED_TELEMETRY_LENGTH; octet = octet + 1)
  {
    const byte data = EEPROM.read(ENABLED_TELEMETRY_OFFSET + octet);
    for (int bit = 0; bit < 8; bit = bit + 1)
    {
      const byte identifier = 8 * octet + bit;
      if (bitRead(data, bit))
      {
        enabledTelemetry.clear(identifier);
      }
      else
      {
        enabledTelemetry.set(identifier);
      }
    }
  }
}

void ESAT_WifiConfigurationClass::readGatewayAddress()
{
  gatewayAddress[0] = EEPROM.read(GATEWAY_ADDRESS_OFFSET);
  gatewayAddress[1] = EEPROM.read(GATEWAY_ADDRESS_OFFSET + 1);
  gatewayAddress[2] = EEPROM.read(GATEWAY_ADDRESS_OFFSET + 2);
  gatewayAddress[3] = EEPROM.read(GATEWAY_ADDRESS_OFFSET + 3);
}

void ESAT_WifiConfigurationClass::readHostAddress()
{
  hostAddress[0]=EEPROM.read(HOST_ADDRESS_OFFSET);
  hostAddress[1]=EEPROM.read(HOST_ADDRESS_OFFSET + 1);
  hostAddress[2]=EEPROM.read(HOST_ADDRESS_OFFSET + 2);
  hostAddress[3]=EEPROM.read(HOST_ADDRESS_OFFSET + 3);
}

void ESAT_WifiConfigurationClass::readHostConfigurationMode()
{
  switch (EEPROM.read(HOST_CONFIGURATION_MODE_OFFSET))
  {
    case 1:
    {
      hostConfigurationMode = ESAT_WifiConfigurationClass::STATIC_HOST_CONFIGURATION_MODE;
      break;
    }
    case 0:
    default:
    {
      hostConfigurationMode = ESAT_WifiConfigurationClass::DYNAMIC_HOST_CONFIGURATION_MODE; // DHCP
      break;
    }
  }
}

void ESAT_WifiConfigurationClass::readHostname()
{
  readString(hostname, HOSTNAME_LENGTH, HOSTNAME_OFFSET);
  hostname[HOSTNAME_LENGTH] = 0; // Ensure that the string is null-terminated.
}

void ESAT_WifiConfigurationClass::readNetworkSSID()
{
  readString(networkSSID, NETWORK_SSID_LENGTH, NETWORK_SSID_OFFSET);
  networkSSID[NETWORK_SSID_LENGTH] = 0; // Ensure that the string is null-terminated.
}

void ESAT_WifiConfigurationClass::readNetworkPassphrase()
{
  readString(networkPassphrase, NETWORK_PASSPHRASE_LENGTH, NETWORK_PASSPHRASE_OFFSET);
  networkPassphrase[NETWORK_PASSPHRASE_LENGTH] = 0; // Ensure that the string is null-terminated.
}

void ESAT_WifiConfigurationClass::readServerAddress()
{
  readString(serverAddress, SERVER_ADDRESS_LENGTH, SERVER_ADDRESS_OFFSET);
  serverAddress[SERVER_ADDRESS_LENGTH] = 0; // Ensure that the string is null-terminated.

}

void ESAT_WifiConfigurationClass::readServerPort()
{
  const byte highByte = EEPROM.read(SERVER_PORT_OFFSET);
  const byte lowByte = EEPROM.read(SERVER_PORT_OFFSET + 1);
  serverPort = word(highByte, lowByte);
}

void ESAT_WifiConfigurationClass::readString(char* outputBuffer, const word length, const word offset)
{
  for (word i = 0; i < length; i = i + 1)
  {
    outputBuffer[i] = EEPROM.read(offset + i);
  }
}

void ESAT_WifiConfigurationClass::readSubnetMask()
{
  subnetMask[0] = EEPROM.read(SUBNET_MASK_OFFSET);
  subnetMask[1] = EEPROM.read(SUBNET_MASK_OFFSET + 1);
  subnetMask[2] = EEPROM.read(SUBNET_MASK_OFFSET + 2);
  subnetMask[3] = EEPROM.read(SUBNET_MASK_OFFSET + 3);
}

void ESAT_WifiConfigurationClass::writeConfiguration()
{
  writeNetworkSSID();
  writeNetworkPassphrase();
  writeServerAddress();
  writeServerPort();
  writeHostAddress();
  writeSubnetMask();
  writeGatewayAddress();
  writeDNSServer1Address();
  writeDNSServer2Address();
  writeHostConfigurationMode();
  writeHostname();
  writeEnabledTelemetry();
}

void ESAT_WifiConfigurationClass::writeDNSServer1Address()
{
  EEPROM.write(DNS_1_ADDRESS_OFFSET, domainNameSystemServer1Address[0]);
  EEPROM.write(DNS_1_ADDRESS_OFFSET + 1, domainNameSystemServer1Address[1]);
  EEPROM.write(DNS_1_ADDRESS_OFFSET + 2, domainNameSystemServer1Address[2]);
  EEPROM.write(DNS_1_ADDRESS_OFFSET + 3, domainNameSystemServer1Address[3]);
  EEPROM.commit();
}

void ESAT_WifiConfigurationClass::writeDNSServer2Address()
{
  EEPROM.write(DNS_2_ADDRESS_OFFSET, domainNameSystemServer2Address[0]);
  EEPROM.write(DNS_2_ADDRESS_OFFSET + 1, domainNameSystemServer2Address[1]);
  EEPROM.write(DNS_2_ADDRESS_OFFSET + 2, domainNameSystemServer2Address[2]);
  EEPROM.write(DNS_2_ADDRESS_OFFSET + 3, domainNameSystemServer2Address[3]);
  EEPROM.commit();
}

void ESAT_WifiConfigurationClass::writeEnabledTelemetry()
{
  // Write the list of enabled telemetry packets to EEPROM.  Bytes
  // that have never been written have all bits set to 1; if we want
  // never-enabled packets to be disabled by default, we must store
  // enabled packets as 0s and disabled packets as 1s.
  for (int octet = 0;
       octet < ENABLED_TELEMETRY_LENGTH;
       octet = octet + 1)
  {
    byte data = 0;
    for (int bit = 0;
         bit < 8;
         bit = bit + 1)
    {
      const int identifier = 8 * octet + bit;
      if (enabledTelemetry.read(identifier))
      {
        bitClear(data, bit);
      }
      else
      {
        bitSet(data, bit);
      }
    }
    EEPROM.write(ENABLED_TELEMETRY_OFFSET + octet, data);
  }
  EEPROM.commit();
}

void ESAT_WifiConfigurationClass::writeGatewayAddress()
{
  EEPROM.write(GATEWAY_ADDRESS_OFFSET, gatewayAddress[0]);
  EEPROM.write(GATEWAY_ADDRESS_OFFSET + 1, gatewayAddress[1]);
  EEPROM.write(GATEWAY_ADDRESS_OFFSET + 2, gatewayAddress[2]);
  EEPROM.write(GATEWAY_ADDRESS_OFFSET + 3, gatewayAddress[3]);
  EEPROM.commit();
}

void ESAT_WifiConfigurationClass::writeHostAddress()
{
  EEPROM.write(HOST_ADDRESS_OFFSET, hostAddress[0]);
  EEPROM.write(HOST_ADDRESS_OFFSET + 1, hostAddress[1]);
  EEPROM.write(HOST_ADDRESS_OFFSET + 2, hostAddress[2]);
  EEPROM.write(HOST_ADDRESS_OFFSET + 3, hostAddress[3]);
  EEPROM.commit();
}

void ESAT_WifiConfigurationClass::writeHostConfigurationMode()
{
  switch (hostConfigurationMode)
  {
    case ESAT_WifiConfigurationClass::STATIC_HOST_CONFIGURATION_MODE:
    {
      EEPROM.write(HOST_CONFIGURATION_MODE_OFFSET, 1);
      break;
    }
    case ESAT_WifiConfigurationClass::DYNAMIC_HOST_CONFIGURATION_MODE: // DHCP
    default:
    {
      EEPROM.write(HOST_CONFIGURATION_MODE_OFFSET, 0);
      break;
    }
  }
  EEPROM.commit();
}

void ESAT_WifiConfigurationClass::writeHostname()
{
  writeString(hostname, HOSTNAME_LENGTH, HOSTNAME_OFFSET);
}

void ESAT_WifiConfigurationClass::writeNetworkPassphrase()
{
  writeString(networkPassphrase, NETWORK_PASSPHRASE_LENGTH, NETWORK_PASSPHRASE_OFFSET);
}

void ESAT_WifiConfigurationClass::writeNetworkSSID()
{
  writeString(networkSSID, NETWORK_SSID_LENGTH, NETWORK_SSID_OFFSET);
}

void ESAT_WifiConfigurationClass::writeServerAddress()
{
  writeString(serverAddress, SERVER_ADDRESS_LENGTH, SERVER_ADDRESS_OFFSET);
}

void ESAT_WifiConfigurationClass::writeServerPort()
{
  EEPROM.write(SERVER_PORT_OFFSET, highByte(serverPort));
  EEPROM.write(SERVER_PORT_OFFSET + 1, lowByte(serverPort));
  EEPROM.commit();
}

void ESAT_WifiConfigurationClass::writeSubnetMask()
{
  EEPROM.write(SUBNET_MASK_OFFSET, subnetMask[0]);
  EEPROM.write(SUBNET_MASK_OFFSET + 1, subnetMask[1]);
  EEPROM.write(SUBNET_MASK_OFFSET + 2, subnetMask[2]);
  EEPROM.write(SUBNET_MASK_OFFSET + 3, subnetMask[3]);
  EEPROM.commit();
}

void ESAT_WifiConfigurationClass::writeString(char* inputBuffer, const word length, const word offset)
{
  for (word i = 0; i < length; i = i + 1)
  {
    EEPROM.write(offset + i, inputBuffer[i]);
  }
  EEPROM.commit();
}

ESAT_WifiConfigurationClass ESAT_WifiConfiguration;
