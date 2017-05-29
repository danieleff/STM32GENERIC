#include <Arduino.h>

#include "stm32_gpio.h"

#include "syscalls.h"

extern "C" void preinitVariant() {
    //Set heap to external SDRAM
    setHeap((unsigned char*)0xC0000000, (unsigned char*)(0xC0000000 + 8 * 1024 * 1024));
}

extern "C" void initVariant() {

    //UART1 is connected to ST-Link V2.1 as Virtual Com port on non-default PA9/PB7 pins
    SerialUART1.stm32SetTX(PA9);
    SerialUART1.stm32SetRX(PB7);
}
