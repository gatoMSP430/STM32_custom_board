/*
  main.cpp - Main loop for Arduino sketches
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#define ARDUINO_MAIN
#include "Arduino.h"

__attribute__((section(".noinit"))) uint32_t boot_flag;
void JumpToBootloader(void){
	__enable_irq();
	HAL_RCC_DeInit();
	HAL_DeInit();
	SysTick->CTRL = SysTick->LOAD = SysTick->VAL = 0;
	__HAL_SYSCFG_REMAPMEMORY_SYSTEMFLASH();

	const uint32_t p = (*((uint32_t *) 0x1FFF0000));
	__set_MSP( p );

	void (*SysMemBootJump)(void);
	SysMemBootJump = (void (*)(void)) (*((uint32_t *) 0x1FFF0004));
	SysMemBootJump();

    while (1);
}
// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated objects that need HAL may fail.
__attribute__((constructor(101))) void premain()
{
  // Required by FreeRTOS, see http://www.freertos.org/RTOS-Cortex-M3-M4.html
#ifdef NVIC_PRIORITYGROUP_4
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
#endif
#if (__CORTEX_M == 0x07U)
  // Defined in CMSIS core_cm7.h
#ifndef I_CACHE_DISABLED
  SCB_EnableICache();
#endif
#ifndef D_CACHE_DISABLED
  SCB_EnableDCache();
#endif
#endif
if (boot_flag == 0xDEADBEEF) {
        boot_flag = 0;   // clear flag
        JumpToBootloader();
    }
  init();
}


/*
 * \brief Main entry point of Arduino application
 */
int main(void)
{
	if (boot_flag == 0xDEADBEEF) {
        boot_flag = 0;   // clear flag
        JumpToBootloader();
    }
  initVariant();

  setup();
  for (;;) {
#if defined(CORE_CALLBACK)
    CoreCallback();
#endif
    loop();
    serialEventRun();
  }

  return 0;
}
