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

#include "stm32_gpio.h"

#define CHANGE 1
#define FALLING 2
#define RISING 3

typedef void (*stm32_exti_callback_func)();

#if defined(STM32F0) || defined(STM32L0)
const uint8_t exti_irq[] = {EXTI0_1_IRQn, EXTI0_1_IRQn, EXTI2_3_IRQn, EXTI2_3_IRQn, EXTI4_15_IRQn,
        EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn,
        EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn, EXTI4_15_IRQn};
#elif defined(STM32F3)
const uint8_t exti_irq[] = {EXTI0_IRQn, EXTI1_IRQn, EXTI2_TSC_IRQn, EXTI3_IRQn, EXTI4_IRQn,
        EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,
        EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn};
#else
const uint8_t exti_irq[] = {EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn,
        EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,
        EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn};
#endif

stm32_exti_callback_func callbacks[16];

void attachInterrupt(uint8_t pin, stm32_exti_callback_func callback, int mode) {
    const stm32_port_pin_type port_pin = variant_pin_list[pin];

    uint8_t irq = __builtin_ffs(port_pin.pin_mask) - 1;
    callbacks[irq] = callback;

    stm32GpioClockEnable(port_pin.port);

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = port_pin.pin_mask;
    switch(mode) {
        case RISING:
            GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
            break;

        case FALLING:
            GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
            break;

        case CHANGE:
            default:
            GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    }

    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(port_pin.port, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(exti_irq[irq], 6, 0);
    HAL_NVIC_EnableIRQ(exti_irq[irq]);
}

void detachInterrupt(uint8_t pin) {
    callbacks[__builtin_ffs(variant_pin_list[pin].pin_mask) - 1] = NULL;
}

void EXTI0_IRQHandler(void) {
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void EXTI1_IRQHandler(void) {
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void EXTI2_IRQHandler(void) {
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI3_IRQHandler(void) {
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void EXTI4_IRQHandler(void) {
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void EXTI9_5_IRQHandler(void) {
  for(uint32_t pin = GPIO_PIN_5; pin <= GPIO_PIN_9; pin=pin<<1) {
    HAL_GPIO_EXTI_IRQHandler(pin);
  }
}

void EXTI15_10_IRQHandler(void) {
  for(uint32_t pin = GPIO_PIN_10; pin <= GPIO_PIN_15; pin=pin<<1) {
    HAL_GPIO_EXTI_IRQHandler(pin);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    stm32_exti_callback_func callback = callbacks[__builtin_ffs(GPIO_Pin) - 1];
    if (callback != NULL) {
        callback();
    }
}
