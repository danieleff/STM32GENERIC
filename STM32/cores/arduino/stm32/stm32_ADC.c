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

/**
 * Implementation of Arduino methods:
 * analogRead(): https://www.arduino.cc/en/Reference/AnalogRead
 *
 * TODO actually create stm32_analog_in_XX.c files for the different variants. This is getting too compicated
 *
 * TODO improve performance by saving if the pin was configured for analog, and do not configure again.
 *   also deconfigure from pinMode in this case
 * TODO implement analogReadResolution
 * TODO F1 has no resolution, map to arduino default 10 bits
 * TODO check if ClockPrescaler can always be set to ADC_CLOCK_SYNC_PCLK_DIV2
 */

#include "stm32_gpio_af.h"

void stm32_adc_init(ADC_HandleTypeDef *handle);

#ifdef ADC_CLOCK_SYNC_PCLK_DIV2
#define ADC_CLOCK_DIV ADC_CLOCK_SYNC_PCLK_DIV2
#elif defined(ADC_CLOCK_ASYNC_DIV1)
#define ADC_CLOCK_DIV ADC_CLOCK_ASYNC_DIV1
#elif defined(ADC_CLOCKPRESCALER_PCLK_DIV2)
#define ADC_CLOCK_DIV ADC_CLOCKPRESCALER_PCLK_DIV2

#elif defined(STM32F1)
#define ADC_CLOCK_DIV
#else
#error "Unknown clock"
#endif

static int readResolution = 10;

void analogReadResolution(int resolution) {
    readResolution = resolution;
}

int analogRead(uint8_t pin) {
    static ADC_HandleTypeDef handle = {};

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = variant_pin_list[pin].pin_mask;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(variant_pin_list[pin].port, &GPIO_InitStruct);

    if (handle.Instance == NULL) {
        #ifdef __HAL_RCC_ADC1_CLK_ENABLE
        __HAL_RCC_ADC1_CLK_ENABLE();
        #endif
        #ifdef __HAL_RCC_ADC_CLK_ENABLE
        __HAL_RCC_ADC_CLK_ENABLE();
        #endif
        
        handle.Instance = ADC1;
        handle.Init.ScanConvMode = DISABLE;
        handle.Init.ContinuousConvMode = ENABLE;
        handle.Init.DiscontinuousConvMode = DISABLE;
        handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;

        #ifdef STM32L0
            handle.Init.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
        #endif

        #if !defined(STM32L0) && !defined(STM32F0)
            handle.Init.NbrOfConversion = 1;
        #endif

        #ifdef ADC_EOC_SINGLE_CONV
            handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
        #endif

        #ifdef STM32F1
            handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
        #else
            handle.Init.ClockPrescaler = ADC_CLOCK_DIV;
            handle.Init.Resolution = ADC_RESOLUTION_12B;
        #endif

        HAL_ADC_Init(&handle);

    }

    ADC_ChannelConfTypeDef sConfig;
    sConfig.Channel = stm32ADC1GetChannel(variant_pin_list[pin].port, variant_pin_list[pin].pin_mask);
    sConfig.Rank = 1;

    #if STM32L0
        //in handle
    #elif defined(ADC_SAMPLETIME_15CYCLES)
        sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
    #elif defined(ADC_SAMPLETIME_13CYCLES_5)
        sConfig.SamplingTime = ADC_SAMPLETIME_13CYCLES_5;
    #elif defined(ADC_SAMPLETIME_19CYCLES_5)
        sConfig.SamplingTime = ADC_SAMPLETIME_19CYCLES_5;
    #elif defined(ADC_SAMPLETIME_16CYCLES)
        sConfig.SamplingTime = ADC_SAMPLETIME_16CYCLES;
    #elif defined(ADC_SAMPLETIME_12CYCLES_5)
        sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;
    #else
        #error "unknown sampleing time"
    #endif

    HAL_ADC_ConfigChannel(&handle, &sConfig);

    HAL_ADC_Start(&handle);

    if (HAL_ADC_PollForConversion(&handle, 1000) != HAL_OK) {
            return 0;
    }

    return (HAL_ADC_GetValue(&handle) << readResolution) >> 12;
}
