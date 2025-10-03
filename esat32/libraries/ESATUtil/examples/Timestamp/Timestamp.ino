/*
 * Copyright (C) 2018 Theia Space, Universidad Politécnica de Madrid
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ESAT_Timestamp.h>

// ESAT_Timestamp example program.
// Timestamp handling.

void setup()
{
  // Configure the Serial interface.
  Serial.begin(9600);
  // Wait until Serial is ready.
  while (!Serial)
  {
  }
}

void loop()
{
  (void) Serial.println(F("##########################"));
  (void) Serial.println(F("Timestamp example program."));
  (void) Serial.println(F("##########################"));
  // First timestamp: launch time of the Apollo 1 mission.
  ESAT_Timestamp firstTimestamp(1969, 7, 16, 13, 32, 0);
  // Second timestamp: just one minute later.
  ESAT_Timestamp secondTimestamp(1969, 7, 16, 13, 33, 0);
  // Print the timestamps.
  (void) Serial.print(F("First timestamp: "));
  (void) Serial.println(firstTimestamp);
  (void) Serial.print(F("Second timestamp: "));
  (void) Serial.println(secondTimestamp);
  // Timestamp comparison.
  if (firstTimestamp < secondTimestamp)
  {
    (void) Serial.println(F("First timestamp < second timestamp."));
  }
  else
  {
    (void) Serial.println(F("First timestamp >= second timestamp."));
  }
  // Timestamp modification.
  (void) Serial.println(F("Adding 100 seconds to the first timestamp."));
  firstTimestamp.addSeconds(100);
  (void) Serial.print(F("New value of the first timestamp: "));
  (void) Serial.println(firstTimestamp);
  if (firstTimestamp > secondTimestamp)
  {
    (void) Serial.println(F("First timestamp > second timestamp."));
  }
  else
  {
    (void) Serial.println(F("First timestamp <= second timestamp."));
  }
  // End.
  (void) Serial.println(F("End."));
  (void) Serial.println();
  delay(1000);
}
