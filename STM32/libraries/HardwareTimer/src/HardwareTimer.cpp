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

#include "HardwareTimer.h"

#include CHIP_PERIPHERAL_INCLUDE

HardwareTimer *interruptTimers[18];

void (*pwm_callback_func)();
static void handleInterrupt(HardwareTimer *timer);

static const uint32_t OCMODE_NOT_USED = 0xFFFF;

HardwareTimer::HardwareTimer(TIM_TypeDef *instance, const stm32_tim_pin_list_type *pin_list, int pin_list_size) {
    this->tim_pin_list = pin_list;
    this->tim_pin_list_size = pin_list_size;

    handle.Instance = instance;

    for(int i=0; i<4; i++) {
        channelOC[i].OCMode = OCMODE_NOT_USED;
        channelOC[i].OCPolarity = TIM_OCPOLARITY_HIGH;
        channelOC[i].OCNPolarity = TIM_OCNPOLARITY_HIGH;
        channelOC[i].OCFastMode = TIM_OCFAST_DISABLE;
        channelOC[i].OCIdleState = TIM_OCIDLESTATE_RESET;
        channelOC[i].OCNIdleState = TIM_OCNIDLESTATE_RESET;

        channelIC[i].ICPolarity = OCMODE_NOT_USED;
        channelIC[i].ICSelection = TIM_ICSELECTION_DIRECTTI;
        channelIC[i].ICPrescaler = TIM_ICPSC_DIV1;
        channelIC[i].ICFilter = 0;
    }

}

void HardwareTimer::pause() {
    HAL_TIM_Base_Stop(&handle);
}

void HardwareTimer::resume() {
    bool hasInterrupt = false;
    for(size_t i=0; i<sizeof(callbacks) / sizeof(callbacks[0]); i++) {
        if (callbacks[i] != NULL) {
            hasInterrupt = true;
            break;
        }
    }

#ifdef TIM1
    if (handle.Instance == TIM1) {
        __HAL_RCC_TIM1_CLK_ENABLE();
        interruptTimers[0] = this;
        if (hasInterrupt) {
            HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

            HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
        }

        TIM_ClockConfigTypeDef sClockSourceConfig;
        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        HAL_TIM_ConfigClockSource(&handle, &sClockSourceConfig);

        TIM_MasterConfigTypeDef sMasterConfig;
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
          sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
          HAL_TIMEx_MasterConfigSynchronization(&handle, &sMasterConfig);

        handle.Init.RepetitionCounter = 0;
    }
#endif

#ifdef TIM2
    if (handle.Instance == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
        pwm_callback_func = []() { handleInterrupt(interruptTimers[1]); };
        interruptTimers[1] = this;
        if (hasInterrupt) {
            HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(TIM2_IRQn);
        }
    }
#endif

#ifdef TIM3
    if (handle.Instance == TIM3) {
        __HAL_RCC_TIM3_CLK_ENABLE();
        interruptTimers[2] = this;
        if (hasInterrupt) {
            HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(TIM3_IRQn);
        }
    }
#endif

#ifdef TIM4
    if (handle.Instance == TIM4) {
        __HAL_RCC_TIM4_CLK_ENABLE();
        interruptTimers[3] = this;
        if (hasInterrupt) {
            HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(TIM4_IRQn);
        }
    }
#endif

    handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    handle.Init.RepetitionCounter = 0;

    HAL_TIM_Base_Init(&handle);

    if (callbacks[0] != NULL) {
        HAL_TIM_Base_Start_IT(&handle);
    } else {

        HAL_TIM_Base_Start(&handle);
    }

    resumeChannel(1, TIM_CHANNEL_1);
    resumeChannel(2, TIM_CHANNEL_2);
    resumeChannel(3, TIM_CHANNEL_3);
    resumeChannel(4, TIM_CHANNEL_4);
}

void HardwareTimer::resumeChannel(int channel, int timChannel) {

    if (channelOC[channel - 1].OCMode != OCMODE_NOT_USED) {
        HAL_TIM_OC_ConfigChannel(&handle, &channelOC[channel - 1], timChannel);

        if (callbacks[channel] != NULL) {
            HAL_TIM_OC_Start_IT(&handle, timChannel);
        } else {
            HAL_TIM_OC_Start(&handle, timChannel);
        }
    }

    if (channelIC[channel - 1].ICPolarity != OCMODE_NOT_USED) {
        HAL_TIM_IC_ConfigChannel(&handle, &channelIC[channel - 1], timChannel);

        if (callbacks[channel] != NULL) {
            HAL_TIM_IC_Start_IT(&handle, timChannel);
        } else {
            HAL_TIM_IC_Start(&handle, timChannel);
        }
    }
}

uint32_t HardwareTimer::getPrescaleFactor() {
    return handle.Init.Prescaler;
}

void HardwareTimer::setPrescaleFactor(uint32_t prescaler) {
    handle.Init.Prescaler = prescaler;
}

uint32_t HardwareTimer::getOverflow() {
    return handle.Init.Period;
}

void HardwareTimer::setOverflow(uint32_t overflow) {
    handle.Init.Period = overflow;
}

uint32_t HardwareTimer::getCount(void) {
    return __HAL_TIM_GET_COUNTER(&handle);
}

void HardwareTimer::setCount(uint32_t counter) {
    __HAL_TIM_SET_COUNTER(&handle, counter);
}

#define MAX_RELOAD ((1 << 16) - 1) // Not always! 32 bit timers!

uint32_t HardwareTimer::getBaseFrequency() {
    int freqMul = 2;
    int freq2Mul = 2;
    uint32_t pFreq = HAL_RCC_GetPCLK1Freq();

#ifdef STM32F1
    freq2Mul = 1;
#endif

#ifdef TIM1
    if (handle.Instance == TIM1) {
        pFreq = HAL_RCC_GetPCLK2Freq();
        freqMul = freq2Mul;
    }
#endif
#ifdef TIM8
    if (handle.Instance == TIM8) {
        pFreq = HAL_RCC_GetPCLK2Freq();
        freqMul = freq2Mul;
    }
#endif
#ifdef TIM9
    if (handle.Instance == TIM9) {
        pFreq = HAL_RCC_GetPCLK2Freq();
        freqMul = freq2Mul;
    }
#endif
#ifdef TIM10
    if (handle.Instance == TIM10) {
        pFreq = HAL_RCC_GetPCLK2Freq();
        freqMul = freq2Mul;
    }
#endif
#ifdef TIM11
    if (handle.Instance == TIM11) {
        pFreq = HAL_RCC_GetPCLK2Freq();
        freqMul = freq2Mul;
    }
#endif

    return pFreq * freqMul;
}

uint32_t HardwareTimer::setPeriod(uint32_t microseconds) {
    if (!microseconds) {
        this->setPrescaleFactor(1);
        this->setOverflow(1);
        return this->getOverflow();
    }

    uint32_t period_cyc = microseconds * (getBaseFrequency()  / 1000000); //TODO!

    uint32_t prescaler = (uint32_t)(period_cyc / MAX_RELOAD + 1);

    uint32_t overflow = (uint32_t)((period_cyc + (prescaler / 2)) / prescaler);

    this->setPrescaleFactor(prescaler);
    this->setOverflow(overflow);
    return overflow;
}

static bool isSameChannel(int channel, uint8_t signal) {
    switch(signal) {
        case TIM_CH1:
        case TIM_CH1N:
            return channel == 1;
        case TIM_CH2:
        case TIM_CH2N:
            return channel == 2;
        case TIM_CH3:
        case TIM_CH3N:
            return channel == 3;
        case TIM_CH4:
        case TIM_CH4N:
            return channel == 4;
    }
    return false;
}

static const uint32_t PIN_NOT_USED = 0xFF;

void HardwareTimer::setMode(int channel, TIMER_MODES mode, uint8_t pin) {
    int pinMode = PIN_NOT_USED;
    int pull = GPIO_NOPULL;

    switch(mode) {
        case TIMER_PWM:
            channelOC[channel - 1].OCMode = TIM_OCMODE_PWM1;
            pinMode = GPIO_MODE_AF_PP;
            break;

        case TIMER_OUTPUT_COMPARE:
            channelOC[channel - 1].OCMode = TIM_OCMODE_TIMING;
            break;

        case TIMER_OUTPUT_COMPARE_ACTIVE:
            channelOC[channel - 1].OCMode = TIM_OCMODE_ACTIVE;
            pinMode = GPIO_MODE_AF_PP;
            break;

        case TIMER_OUTPUT_COMPARE_INACTIVE:
            channelOC[channel - 1].OCMode = TIM_OCMODE_ACTIVE;
            pinMode = GPIO_MODE_AF_PP;
            break;

        case TIMER_OUTPUT_COMPARE_TOGGLE:
            channelOC[channel - 1].OCMode = TIM_OCMODE_TOGGLE;
            pinMode = GPIO_MODE_AF_PP;
            break;

        case TIMER_OUTPUT_COMPARE_PWM1:
            channelOC[channel - 1].OCMode = TIM_OCMODE_PWM1;
            pinMode = GPIO_MODE_AF_PP;
            break;

        case TIMER_OUTPUT_COMPARE_PWM2:
            channelOC[channel - 1].OCMode = TIM_OCMODE_PWM2;
            pinMode = GPIO_MODE_AF_PP;
            break;

        case TIMER_OUTPUT_COMPARE_FORCED_ACTIVE:
            channelOC[channel - 1].OCMode = TIM_OCMODE_FORCED_ACTIVE;
            pinMode = GPIO_MODE_AF_PP;
            break;

        case TIMER_OUTPUT_COMPARE_FORCED_INACTIVE:
            channelOC[channel - 1].OCMode = TIM_OCMODE_FORCED_INACTIVE;
            pinMode = GPIO_MODE_AF_PP;
            break;

        case TIMER_INPUT_CAPTURE_RISING:
            channelIC[channel - 1].ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
            pinMode = GPIO_MODE_AF_PP;
            pull = GPIO_PULLDOWN;
            break;

        case TIMER_INPUT_CAPTURE_FALLING:
            channelIC[channel - 1].ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
            pinMode = GPIO_MODE_AF_PP;
            pull = GPIO_PULLDOWN;
            break;
    }

    if (pinMode != PIN_NOT_USED) {
        for(int i=0; i<tim_pin_list_size; i++) {
            if (isSameChannel(channel, tim_pin_list[i].signalType)) {

                if (pin == TIMER_DEFAULT_PIN ||
                        (variant_pin_list[pin].port == tim_pin_list[i].port && variant_pin_list[pin].pin_mask == tim_pin_list[i].pinMask)) {

                    stm32GpioClockEnable(tim_pin_list[i].port);

                    GPIO_InitTypeDef GPIO_InitStruct;
                    GPIO_InitStruct.Pin = tim_pin_list[i].pinMask;
                    GPIO_InitStruct.Mode = pinMode;
                    GPIO_InitStruct.Pull = pull;
                    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

                    #ifdef STM32F1
                        tim_pin_list[i].alternate();
                    #else
                        GPIO_InitStruct.Alternate = tim_pin_list[i].alternate;
                    #endif

                    HAL_GPIO_Init(tim_pin_list[i].port, &GPIO_InitStruct);

                    return;
                }
            }
        }
    }
}

void HardwareTimer::setCompare(int channel, uint32_t compare) {
    channelOC[channel - 1].Pulse = compare;
}

uint32_t HardwareTimer::getCompare(int channel) {
    if (channel == 1) return __HAL_TIM_GET_COMPARE(&handle, TIM_CHANNEL_1);
    if (channel == 2) return __HAL_TIM_GET_COMPARE(&handle, TIM_CHANNEL_2);
    if (channel == 3) return __HAL_TIM_GET_COMPARE(&handle, TIM_CHANNEL_3);
    if (channel == 4) return __HAL_TIM_GET_COMPARE(&handle, TIM_CHANNEL_4);
    return 0;
}

void HardwareTimer::attachInterrupt(void (*callback)(void)) {
    callbacks[0] = callback;
}

void HardwareTimer::detachInterrupt() {
    callbacks[0] = NULL;
}

void HardwareTimer::attachInterrupt(int channel, void (*callback)(void)) {
    callbacks[channel] = callback;
}

void HardwareTimer::detachInterrupt(int channel) {
    callbacks[channel] = NULL;
}

void HardwareTimer::refresh() {
    HAL_TIM_GenerateEvent(&handle, TIM_EVENTSOURCE_UPDATE);
}

static void handleInterrupt(HardwareTimer *timer) {
    if(__HAL_TIM_GET_FLAG(&timer->handle, TIM_FLAG_UPDATE) != RESET) {
        __HAL_TIM_CLEAR_IT(&timer->handle, TIM_IT_UPDATE);
        if (timer->callbacks[0] != NULL) timer->callbacks[0]();
    }

    if(__HAL_TIM_GET_FLAG(&timer->handle, TIM_FLAG_CC1) != RESET) {
        __HAL_TIM_CLEAR_IT(&timer->handle, TIM_IT_CC1);
        if (timer->callbacks[1] != NULL) timer->callbacks[1]();
    }

    if(__HAL_TIM_GET_FLAG(&timer->handle, TIM_FLAG_CC2) != RESET) {
        __HAL_TIM_CLEAR_IT(&timer->handle, TIM_IT_CC2);
        if (timer->callbacks[2] != NULL) timer->callbacks[2]();
    }

    if(__HAL_TIM_GET_FLAG(&timer->handle, TIM_FLAG_CC3) != RESET) {
        __HAL_TIM_CLEAR_IT(&timer->handle, TIM_IT_CC3);
        if (timer->callbacks[3] != NULL) timer->callbacks[3]();
    }

    if(__HAL_TIM_GET_FLAG(&timer->handle, TIM_FLAG_CC4) != RESET) {
        __HAL_TIM_CLEAR_IT(&timer->handle, TIM_IT_CC4);
        if (timer->callbacks[4] != NULL) timer->callbacks[4]();
    }
}

#ifdef TIM1
    HardwareTimer Timer1(TIM1, chip_tim1, sizeof(chip_tim1) / sizeof(chip_tim1[0]));
#endif

#ifdef TIM2
    HardwareTimer Timer2(TIM2, chip_tim2, sizeof(chip_tim2) / sizeof(chip_tim2[0]));
#endif

#ifdef TIM3
    HardwareTimer Timer3(TIM3, chip_tim3, sizeof(chip_tim3) / sizeof(chip_tim3[0]));
#endif

#ifdef TIM4
    HardwareTimer Timer4(TIM4, chip_tim4, sizeof(chip_tim4) / sizeof(chip_tim4[0]));
#endif


//Timer interrupts:

extern "C" void TIM1_CC_IRQHandler(void) {
    if (interruptTimers[0] != NULL) handleInterrupt(interruptTimers[0]);
}
extern "C" void TIM1_UP_IRQHandler(void) {
    if (interruptTimers[0] != NULL) handleInterrupt(interruptTimers[0]);
}
#ifndef TIM1_UP_TIM10_IRQHandler
    extern "C" void TIM1_UP_TIM10_IRQHandler(void) {
        if (interruptTimers[0] != NULL) handleInterrupt(interruptTimers[0]);
        if (interruptTimers[9] != NULL) handleInterrupt(interruptTimers[9]);
    }
#endif

// in stm32_PWM.c
/*
extern "C" void TIM2_IRQHandler(void) {
    handleInterrupt(interruptTimers[1]);
}*/

extern "C" void TIM3_IRQHandler(void) {
    if (interruptTimers[2] != NULL) handleInterrupt(interruptTimers[2]);
}
extern "C" void TIM4_IRQHandler(void) {
    if (interruptTimers[3] != NULL) handleInterrupt(interruptTimers[3]);
}
