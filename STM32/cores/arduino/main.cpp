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

#include <Arduino.h>
#include "USBDevice.h"

// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (* /*func*/ )()) { return 0; }

// Weak empty variant initialization function.
// May be redefined by variant files.
void initVariant() __attribute__((weak));
void initVariant() { }

void setupUSB() __attribute__((weak));
void setupUSB() { }

// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated objects that need HAL may fail.
 __attribute__(( constructor (101))) void premain() {

// Required by FreeRTOS, see http://www.freertos.org/RTOS-Cortex-M3-M4.html
#ifdef NVIC_PRIORITYGROUP_4
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
#endif

    init();
}

int main(void)
{
    #ifdef STM32F7
    SCB_EnableICache();
    SCB_EnableDCache();
    #endif

	initVariant();

    #if defined(MENU_DEBUG_DISABLED)
        __HAL_AFIO_REMAP_SWJ_DISABLE();
    #elif defined(MENU_DEBUG_SWD)
        __HAL_AFIO_REMAP_SWJ_NOJTAG();
    #elif defined(MENU_DEBUG_JTAG)
        __HAL_AFIO_REMAP_SWJ_ENABLE();
    #endif

    #if defined(USB_BASE) || defined(USB_OTG_DEVICE_BASE)
        setupUSB();
    #ifdef MENU_USB_SERIAL
        USBDeviceFS.beginCDC();
    #elif MENU_USB_MASS_STORAGE
        USBDeviceFS.beginMSC();
    #endif

    #endif

	setup();

	for (;;) {
		loop();
	}

	return 0;
}
