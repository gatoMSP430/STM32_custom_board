/*
 * Copyright (C) 2019 Theia Space, Universidad Politécnica de Madrid
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

#include "ESAT_OBC-telemetry/ESAT_OBCProcessorTelemetry.h"
//#include <ProcessorTemperature.h>
//#include <ProcessorVoltage.h>
#include <RAMStatistics.h>
#include "stm32yyxx_ll_adc.h"

float ESAT_OBCProcessorTelemetryClass::readVref()
{
	analogReadResolution(12);
	return float(__LL_ADC_CALC_VREFANALOG_VOLTAGE(analogRead(AVREF), LL_ADC_RESOLUTION_12B))/1000;
}
float ESAT_OBCProcessorTelemetryClass::readTempSensor()
{
	analogReadResolution(12);
	uint32_t Vref=__LL_ADC_CALC_VREFANALOG_VOLTAGE(analogRead(AVREF), LL_ADC_RESOLUTION_12B);
	return __LL_ADC_CALC_TEMPERATURE(Vref, analogRead(ATEMP), LL_ADC_RESOLUTION_12B);
}

float ESAT_OBCProcessorTelemetryClass::readVBat()
{
	analogReadResolution(12);
    uint32_t Vref=__LL_ADC_CALC_VREFANALOG_VOLTAGE(analogRead(AVREF), LL_ADC_RESOLUTION_12B);
	return float(__LL_ADC_CALC_DATA_TO_VOLTAGE(Vref, AVBAT, LL_ADC_RESOLUTION_12B))/1000;
}

boolean ESAT_OBCProcessorTelemetryClass::available()
{
  // The OBC housekeeping telemetry packet is always available.
  return true;
}

boolean ESAT_OBCProcessorTelemetryClass::fillUserData(ESAT_CCSDSPacket& packet)
{
  packet.writeUnsignedLong(millis());
  packet.writeFloat(readTempSensor());
  packet.writeFloat( readVref());
  packet.writeFloat(RAMStatistics.currentUsagePercentage());
  packet.writeFloat(RAMStatistics.maximumUsagePercentage());
  return true;
}

ESAT_OBCProcessorTelemetryClass ESAT_OBCProcessorTelemetry;
