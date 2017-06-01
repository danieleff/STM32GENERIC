/*
  Copyright (c) 2017 Daniel Fekete

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef STM32_GPIO_H
#define STM32_GPIO_H

#include "stm32_def.h"

#include "stm32_pin_list.h"

#include "variant.h"

#include "stm32_HAL/stm32XXxx_ll_gpio.h"

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define INPUT_PULLDOWN 0x3

#ifndef GPIO_SPEED_FREQ_VERY_HIGH
    #define GPIO_SPEED_FREQ_VERY_HIGH GPIO_SPEED_FREQ_HIGH
#endif

#define RwReg uint32_t


#ifdef __cplusplus
extern "C"{
#endif

typedef struct {
    GPIO_TypeDef *port;
    uint32_t pin_mask;
} stm32_port_pin_type;

typedef uint32_t (*stm32_clock_freq_func)();

typedef struct {
    void *instance;
    stm32_clock_freq_func clock_freq_func;
} stm32_clock_freq_list_type;

extern const stm32_port_pin_type variant_pin_list[NUM_PINS];

/**
 * Start clock for the fedined port
 */
void stm32GpioClockEnable(GPIO_TypeDef *port);

/**
 * If PWM is used at least once, this method is set to the PWM disable function in stm32_PWM.c
 */
typedef void (*stm32_pwm_disable_callback_func)(GPIO_TypeDef *port, uint32_t pin);
extern stm32_pwm_disable_callback_func stm32_pwm_disable_callback;

inline void digitalWrite(uint8_t pin, uint8_t value) {
    if (pin >= sizeof(variant_pin_list) / sizeof(variant_pin_list[0])) {
        return;
    }
    
    stm32_port_pin_type port_pin = variant_pin_list[pin];
    
    HAL_GPIO_WritePin(port_pin.port, port_pin.pin_mask, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
    
}


inline int digitalRead(uint8_t pin) {
    if (pin >= sizeof(variant_pin_list) / sizeof(variant_pin_list[0])) {
        return 0;
    }
    
    stm32_port_pin_type port_pin = variant_pin_list[pin];
    
    return HAL_GPIO_ReadPin(port_pin.port, port_pin.pin_mask);
    
}

#ifdef __cplusplus
}
#endif


///////////////////////////////
// The following functions are meant to be used with compile time constant parameters

#define PIN(a, b) { GPIO##a , LL_GPIO_PIN_##b }
static const stm32_port_pin_type variant_pin_list_ll_static[] = {
  PIN_LIST
};
#undef PIN

#ifdef __cplusplus

inline void digitalWrite(__ConstPin pin, uint8_t value) {
    if (value) {
        LL_GPIO_SetOutputPin(variant_pin_list_ll_static[pin].port, variant_pin_list_ll_static[pin].pin_mask);
    } else {
        LL_GPIO_ResetOutputPin(variant_pin_list_ll_static[pin].port, variant_pin_list_ll_static[pin].pin_mask);
    }
}

inline int digitalRead(__ConstPin pin) {
    return LL_GPIO_IsInputPinSet(variant_pin_list_ll_static[pin].port, variant_pin_list_ll_static[pin].pin_mask);
}

extern "C" void pinModeLL(GPIO_TypeDef *port, uint32_t ll_pin, uint8_t mode);

inline static void pinMode(__ConstPin pin, uint8_t mode) {
    pinModeLL(variant_pin_list_ll_static[pin].port, variant_pin_list_ll_static[pin].pin_mask, mode);
}

#endif


#endif
