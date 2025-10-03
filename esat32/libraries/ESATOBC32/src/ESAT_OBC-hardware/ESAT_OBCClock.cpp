/*
 * Copyright (C) 2017, 2018 Theia Space, Universidad Politécnica de Madrid
 *
 * This file is part of Theia Space's ESAT OBC library.
 *
 * Theia Space's ESAT OBC library is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Theia Space's ESAT OBC library is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Theia Space's ESAT OBC library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "ESAT_OBC-hardware/ESAT_OBCClock.h"
#include <STM32RTC.h>
STM32RTC& rtc = STM32RTC::getInstance();

ESAT_Timestamp ESAT_OBCClockClass::read()
{
 ESAT_Timestamp timeToBeRead; // ESAT_Timestamp return object.
  timeToBeRead.seconds = (byte) rtc.getSeconds();
  timeToBeRead.minutes = (byte) rtc.getMinutes();
  timeToBeRead.hours = (byte) rtc.getHours();
  timeToBeRead.day = (byte) rtc.getDay();
  timeToBeRead.month = (byte) rtc.getMonth();
  // Yikes! Year 2000 problem strikes again!
  timeToBeRead.year = (word) (rtc.getYear());
//    + rtc.read(YEAR_BACKUP_REGISTER) * 100);
  return timeToBeRead;
}

void ESAT_OBCClockClass::write(ESAT_Timestamp timeToSet)
{
 rtc.setSeconds((byte) timeToSet.seconds);
  rtc.setMinutes((byte) timeToSet.minutes);
  rtc.setHours((byte) timeToSet.hours);
  rtc.setDay((byte) timeToSet.day);
  rtc.setMonth((byte) timeToSet.month);
  // RTC doesn't handle century and millenium digits, so we store them
  // in a backup register.
  rtc.setYear((byte) (timeToSet.year % 100));
  //rtc.write(YEAR_BACKUP_REGISTER, timeToSet.year / 100);
}

ESAT_OBCClockClass ESAT_OBCClock;
