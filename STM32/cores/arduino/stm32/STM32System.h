#ifndef STM32_SYSTEM_H
#define STM32_SYSTEM_H

#include "Arduino.h"

// Returns if we are in an interrupt
inline bool isInterrupt() {
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0 ;
}

// Internal: what will be the
extern "C" int stm32SetPrintOutput(Print *p);

#endif
