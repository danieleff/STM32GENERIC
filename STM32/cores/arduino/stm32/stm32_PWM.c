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

TIM_HandleTypeDef *handle;

static uint32_t counter;
static uint32_t waitCycles;
static uint8_t analogWriteResolutionBits = 8;

const uint32_t TIMER_MAX_CYCLES = UINT16_MAX;

const uint32_t PWM_FREQUENCY_HZ = 1000;

extern void pinMode(uint8_t, uint8_t);

#define min(a,b) ((a)<(b)?(a):(b))

stm32_pwm_disable_callback_func stm32_pwm_disable_callback = NULL;

void (*pwm_callback_func)();

void pwm_callback();

typedef struct {
    GPIO_TypeDef *port;
    void (*callback)();
    uint32_t pin_mask;
    uint32_t waveLengthCycles;
    uint32_t dutyCycle;
    int32_t counterCycles;
} stm32_pwm_type;

static stm32_pwm_type pwm_config[sizeof(variant_pin_list) / sizeof(variant_pin_list[0])];

void stm32_pwm_disable(GPIO_TypeDef *port, uint32_t pin);

void analogWriteResolution(int bits) {
    analogWriteResolutionBits = bits;
}

static void initTimer() {
    static TIM_HandleTypeDef staticHandle;

    if (handle == NULL) {
        handle = &staticHandle;
        pwm_callback_func = &pwm_callback;

        stm32_pwm_disable_callback = &stm32_pwm_disable;


        #ifdef TIM2 //99% of chips have TIM2
            __HAL_RCC_TIM2_CLK_ENABLE();
            HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(TIM2_IRQn);

            handle->Instance = TIM2;
        #else
            __HAL_RCC_TIM3_CLK_ENABLE();
            HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(TIM3_IRQn);

            handle->Instance = TIM3;
        #endif

        handle->Init.Prescaler = 0;
        handle->Init.CounterMode = TIM_COUNTERMODE_UP;
        waitCycles = TIMER_MAX_CYCLES;
        handle->Init.Period = waitCycles;
        handle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        HAL_TIM_Base_Init(handle);

        HAL_TIM_Base_Start_IT(handle);
    }
}

void pwmWrite(uint8_t pin, int dutyCycle, int frequency, int durationMillis) {
    initTimer();

    for(size_t i=0; i<sizeof(pwm_config) / sizeof(pwm_config[0]); i++) {
        if (pwm_config[i].port == NULL ||
                (pwm_config[i].port == variant_pin_list[pin].port
                && pwm_config[i].pin_mask == variant_pin_list[pin].pin_mask)) {

            if (pwm_config[i].port == NULL) {
                pinMode(pin, OUTPUT);
            }

            #ifdef STM32F0 // TODO better condition for when there is no pclk2
                uint32_t timerFreq = HAL_RCC_GetPCLK1Freq();
            #else
                uint32_t timerFreq = HAL_RCC_GetPCLK2Freq();
            #endif

            pwm_config[i].port = variant_pin_list[pin].port;
            pwm_config[i].pin_mask = variant_pin_list[pin].pin_mask;
            pwm_config[i].waveLengthCycles = timerFreq / frequency;
            pwm_config[i].dutyCycle = (uint64_t)pwm_config[i].waveLengthCycles * dutyCycle >> 16;

            if (durationMillis > 0) {
                pwm_config[i].counterCycles = timerFreq / 1000 * durationMillis;
            }

            break;
        }
    }
}

extern void tone(uint8_t pin, unsigned int frequency, unsigned long durationMillis) {
    pwmWrite(pin, 1 << 15, frequency, durationMillis);
}

void analogWrite(uint8_t pin, int value) {
    pwmWrite(pin, ((uint32_t)value << 16) >> analogWriteResolutionBits, PWM_FREQUENCY_HZ, 0);
}

void stm32ScheduleMicros(uint32_t microseconds, void (*callback)()) {
    initTimer();

    for(size_t i=0; i<sizeof(pwm_config) / sizeof(pwm_config[0]); i++) {
        if (pwm_config[i].port == NULL && pwm_config[i].callback == NULL) {

            pwm_config[i].callback = callback;

            pwm_config[i].waveLengthCycles = HAL_RCC_GetPCLK2Freq() * (uint64_t)microseconds / 1000000;
            pwm_config[i].counterCycles = pwm_config[i].waveLengthCycles;
            break;
        }
    }
}

void stm32_pwm_disable(GPIO_TypeDef *port, uint32_t pin_mask) {
    for(size_t i=0; i<sizeof(pwm_config) / sizeof(pwm_config[0]); i++) {
        if (pwm_config[i].port == NULL) {
            return;
        }

        if (pwm_config[i].port == port && pwm_config[i].pin_mask == pin_mask) {

            for(size_t j = i + 1; j < sizeof(pwm_config) / sizeof(pwm_config[0]); j++) {
                if (pwm_config[j].port == NULL) {
                    pwm_config[i].port = pwm_config[j - 1].port;
                    pwm_config[i].pin_mask = pwm_config[j - 1].pin_mask;

                    pwm_config[j - 1].port = NULL;
                    break;
                }
            }

            break;
        }
    }
}

void pwm_callback() {
    if(__HAL_TIM_GET_FLAG(handle, TIM_FLAG_UPDATE) != RESET) {
        if(__HAL_TIM_GET_IT_SOURCE(handle, TIM_IT_UPDATE) !=RESET) {
            __HAL_TIM_CLEAR_IT(handle, TIM_IT_UPDATE);

            counter += waitCycles;
            uint32_t nextWaitCycles = TIMER_MAX_CYCLES;

            for(size_t i=0; i<sizeof(pwm_config); i++) {
                if (pwm_config[i].port != NULL) {
                    if (pwm_config[i].dutyCycle > counter % pwm_config[i].waveLengthCycles) {
                        pwm_config[i].port->BSRR = pwm_config[i].pin_mask;
                        nextWaitCycles = min(nextWaitCycles, pwm_config[i].dutyCycle - (counter % pwm_config[i].waveLengthCycles));
                    } else {
                        pwm_config[i].port->BSRR = pwm_config[i].pin_mask << 16;
                        nextWaitCycles = min(nextWaitCycles, pwm_config[i].waveLengthCycles - counter % pwm_config[i].waveLengthCycles);
                    }

                    if (pwm_config[i].counterCycles > 0) {
                        if (pwm_config[i].counterCycles <= (int)waitCycles) {
                            stm32_pwm_disable(pwm_config[i].port, pwm_config[i].pin_mask);
                        } else {
                            pwm_config[i].counterCycles -= waitCycles;
                        }
                    }
                } else if (pwm_config[i].callback != NULL) {
                    pwm_config[i].counterCycles -= waitCycles;
                    if (pwm_config[i].counterCycles < 0) {
                        pwm_config[i].callback();
                        pwm_config[i].counterCycles += pwm_config[i].waveLengthCycles;
                    }
                } else {
                    break;
                }
            }

            if (!nextWaitCycles || nextWaitCycles > TIMER_MAX_CYCLES) {
                nextWaitCycles = TIMER_MAX_CYCLES;
            }
            waitCycles = nextWaitCycles;

            __HAL_TIM_SET_AUTORELOAD(handle, waitCycles);
        }
    }
}

#ifdef TIM2
  extern void TIM2_IRQHandler(void) {
#else
  extern void TIM3_IRQHandler(void) {
#endif

    if (pwm_callback_func != NULL) {
        (*pwm_callback_func)();
    }
}
