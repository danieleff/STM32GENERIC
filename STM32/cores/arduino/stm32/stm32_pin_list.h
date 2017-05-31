#ifndef STM32_PIN_LIST_H
#define STM32_PIN_LIST_H

#include "variant.h"

#include "stm32_HAL/stm32XXxx_ll_gpio.h"

#ifdef VARIANT_PIN_LIST
  #define PIN_LIST VARIANT_PIN_LIST
#else
  #define PIN_LIST CHIP_PIN_LIST
#endif

#ifdef __cplusplus

    class __ConstPin {
    public:
      constexpr __ConstPin(const int val): val(val) {};
      constexpr operator int() const { return val; }
      const int val;
    };
    #define PIN(a, b) __P##a##b
        enum {
            PIN_LIST
            NUM_PINS,
        };
    #undef PIN

    #define PIN(a, b) P##a##b(__P##a##b)
      constexpr __ConstPin PIN_LIST __IGNORE(-1);
    #undef PIN

#else

    #define PIN(a, b) P ## a ## b
    enum {
        PIN_LIST
        NUM_PINS,
    };
    #undef PIN

#endif


#endif
