/*
 * Copyright (C) 2019 Theia Space, Universidad Politécnica de Madrid
 *
 * This file is part of Theia Space's Battery Voltage (STM32L476)
 * library.
 *
 * Theia Space's Battery Voltage (STM32L476) is free software: you
 * can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Theia Space's Battery Voltage (STM32L476) library is distributed
 * in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Theia Space's Battery Voltage (STM32L476) library.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "BatteryVoltage.h"

float BatteryVoltageClass::read()
{
  return STM32.getVBAT();
}

word BatteryVoltageClass::readRaw()
{
  return 0;
}

// Global instance of the library.
BatteryVoltageClass BatteryVoltage;
