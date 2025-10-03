/*
 * ESAT OBC Wifi Programmer 1.0.1
 * Copyright (C) 2019 Theia Space, Universidad Politécnica de Madrid.
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
 * along with Theia Space's ESAT OBC Wifi Programmer.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

// ESAT OBC firmware for programming the ESAT Wifi board.

enum ProgrammingProcessState
{
  WAITING_FOR_NEW_TRANSFER,
  TRANSFER_IN_PROGRESS,
};

ProgrammingProcessState programmingProcessState;

const unsigned long RESET_WAIT_MILLISECONDS = 100;
const unsigned long SERIAL_SPEED = 115200;
const unsigned long TIMEOUT_MILLISECONDS = 1000;
unsigned long latestTransferTime;

void disableWifiBoard()
{
  digitalWrite(LED_O, HIGH);
  digitalWrite(ESPRST, LOW);
  delay(RESET_WAIT_MILLISECONDS);
  digitalWrite(LED_O, LOW);
}

boolean longTimeSinceLatestTransfer()
{
  if ((millis() - latestTransferTime) > TIMEOUT_MILLISECONDS)
  {
    return true;
  }
  else
  {
    return false;
  }
}

boolean newProgrammingProcessHasStarted()
{
  if (Serial.available() > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void switchWifiBoardToProgrammingMode()
{
  digitalWrite(LED_O, HIGH);
  digitalWrite(ESPRST, LOW);
  delay(RESET_WAIT_MILLISECONDS);
  SerialWifi.begin(SERIAL_SPEED);
  pinMode(ESP0, OUTPUT);
  digitalWrite(ESP0, LOW);
  delay(RESET_WAIT_MILLISECONDS);
  digitalWrite(ESPRST, HIGH);
  delay(RESET_WAIT_MILLISECONDS);
  pinMode(ESP0, INPUT);
  digitalWrite(LED_O, LOW);
  latestTransferTime = millis();
}

// If there is a byte available at the input Stream,
// copy it to the output Stream and blink the OBC LED.
void transfer(Stream& input, Stream& output)
{
  if (input.available() > 0)
  {
    digitalWrite(LED_O, HIGH);
    (void) output.write(input.read());
    digitalWrite(LED_O, LOW);
    latestTransferTime = millis();
  }
}

// Configure the communication peripherals.
void setup()
{
  (void) Serial.begin(SERIAL_SPEED);
  (void) SerialWifi.begin(SERIAL_SPEED);
  pinMode(LED_O, OUTPUT);
  pinMode(ESPRST, OUTPUT);
  disableWifiBoard();
  programmingProcessState = WAITING_FOR_NEW_TRANSFER;
}

// Programming process state machine.
void loop()
{
  switch (programmingProcessState)
  {
    case WAITING_FOR_NEW_TRANSFER:
      if (newProgrammingProcessHasStarted())
      {
        switchWifiBoardToProgrammingMode();
        programmingProcessState = TRANSFER_IN_PROGRESS;
      }
      else
      {
        programmingProcessState = WAITING_FOR_NEW_TRANSFER;
      }
      break;
    case TRANSFER_IN_PROGRESS:
      transfer(Serial, SerialWifi);
      transfer(SerialWifi, Serial);
      if (longTimeSinceLatestTransfer())
      {
        disableWifiBoard();
        programmingProcessState = WAITING_FOR_NEW_TRANSFER;
      }
      else
      {
        programmingProcessState = TRANSFER_IN_PROGRESS;
      }
      break;
    default:
      break;
  }
}
