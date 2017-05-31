#ifndef STM32_PIN_LIST_H
#define STM32_PIN_LIST_H

#include "variant.h"

#ifdef VARIANT_PIN_LIST
  #define PIN_LIST VARIANT_PIN_LIST
#else
  #define PIN_LIST CHIP_PIN_LIST
#endif


#define PIN(a, b) P ## a ## b
enum {
    PIN_LIST
    NUM_PINS,
};
#undef PIN


#endif
