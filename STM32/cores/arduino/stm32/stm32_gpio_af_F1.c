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

#if defined(STM32F1)

#include "stm32_gpio_af.h"

stm32_af_callback stm32AfGet(const stm32_af_pin_list_type list[], int size, const void *instance, const GPIO_TypeDef *port, const uint32_t pin) {
    for(int i=0; i<size; i++) {
        if (instance == list[i].instance
            && port == list[i].port
            && pin == list[i].pin) {
                
            return list[i].alternate;
        }
    }
    return 0;
}

void stm32AfInitSpeed(const stm32_af_pin_list_type list[], int size, const void *instance, GPIO_TypeDef *port, uint32_t pin, uint32_t mode, uint32_t pull, uint32_t speed) {
    if (stm32_pwm_disable_callback != NULL) {
        (*stm32_pwm_disable_callback)(port, pin);
    }
    if (port == NULL) {
        port = stm32AfGetDefault(list, size, instance, &pin);
    }
    stm32GpioClockEnable(port);
    
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Pull = pull;
    GPIO_InitStruct.Speed = speed;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
    
    stm32AfGet(list, size, instance, port, pin)();
}

#endif
