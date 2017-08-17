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
 * Function to set up GPIO alternate functions for pins.
 *
 * If a port parameter is NULL, the default port/pin will be used.
 */
#ifndef STM32_GPIO_AF_H
#define STM32_GPIO_AF_H

#include "stm32_gpio.h"

typedef enum {
    TIM_BKIN, TIM_BKIN2,
    TIM_CH1, TIM_CH1N,
    TIM_CH2, TIM_CH2N,
    TIM_CH3, TIM_CH3N,
    TIM_CH4, TIM_CH4N,
    TIM_ETR
} TIMER_SIGNALS;

#ifdef STM32F1

typedef void (*stm32_af_callback)();

typedef struct {
    void *instance;
    GPIO_TypeDef *port;
    uint32_t pin;
    stm32_af_callback alternate;
} stm32_af_pin_list_type;

typedef struct {
    void *instance;
    GPIO_TypeDef *port;
    uint32_t pinMask;
    uint8_t signalType;
    stm32_af_callback alternate;
} stm32_tim_pin_list_type;

#else

typedef struct {
    void *instance;
    GPIO_TypeDef *port;
    uint32_t pin;
    uint8_t alternate;
} stm32_af_pin_list_type;

typedef struct {
    void *instance;
    GPIO_TypeDef *port;
    uint32_t pinMask;
    uint8_t signalType;
    uint8_t alternate;
} stm32_tim_pin_list_type;

#endif


typedef struct {
    ADC_TypeDef *instance;
    GPIO_TypeDef *port;
    uint32_t pin_mask;
    uint32_t channel;
} stm32_chip_adc1_channel_type;


#ifdef __cplusplus
extern "C"{
#endif

void stm32AfUARTInit(const USART_TypeDef *instance,
    GPIO_TypeDef *rxPort, uint32_t rxPin, 
    GPIO_TypeDef *txPort, uint32_t txPin);

void stm32AfSPIInit(const SPI_TypeDef *instance,
    GPIO_TypeDef *mosiPort, uint32_t mosiPin,
    GPIO_TypeDef *misoPort, uint32_t misoPin,
	GPIO_TypeDef *sckPort, uint32_t sckPin);

SPI_TypeDef *stm32GetSPIInstance(GPIO_TypeDef *mosiPort, uint32_t mosiPin,
    GPIO_TypeDef *misoPort, uint32_t misoPin,
    GPIO_TypeDef *sckPort, uint32_t sckPin);

void stm32AfI2SInit(const SPI_TypeDef *instance,
    GPIO_TypeDef *sdPort, uint32_t sdPin,
    GPIO_TypeDef *wsPort, uint32_t wsPin,
    GPIO_TypeDef *ckPort, uint32_t ckPin);

void stm32AfI2SInitWithMck(const SPI_TypeDef *instance,
    GPIO_TypeDef *sdPort, uint32_t sdPin,
    GPIO_TypeDef *wsPort, uint32_t wsPin,
    GPIO_TypeDef *ckPort, uint32_t ckPin,
    GPIO_TypeDef *mckPort, uint32_t mckPin);

void stm32AfI2CInit(const I2C_TypeDef *instance,
    GPIO_TypeDef *sdaPort, uint32_t sdaPin,
    GPIO_TypeDef *sclPort, uint32_t sclPin);

#if defined(SDIO) || defined(SDMMC1)

#ifndef SD_TypeDef
#define SD_TypeDef SDIO_TypeDef
#endif

void stm32AfSDIO1BitInit(const SD_TypeDef *instance,
    GPIO_TypeDef *ckPort, uint32_t ckPin,
    GPIO_TypeDef *cmdPort, uint32_t cmdPin,
    GPIO_TypeDef *d0Port, uint32_t d0Pin);

void stm32AfSDIO4BitInit(const SD_TypeDef *instance,
    GPIO_TypeDef *ckPort, uint32_t ckPin,
    GPIO_TypeDef *cmdPort, uint32_t cmdPin,
    GPIO_TypeDef *d0Port, uint32_t d0Pin,
    GPIO_TypeDef *d1Port, uint32_t d1Pin,
    GPIO_TypeDef *d2Port, uint32_t d2Pin,
    GPIO_TypeDef *d3Port, uint32_t d3Pin);

void stm32AfSDIO8BitInit(const SD_TypeDef *instance,
    GPIO_TypeDef *ckPort, uint32_t ckPin,
    GPIO_TypeDef *cmdPort, uint32_t cmdPin,
    GPIO_TypeDef *d0Port, uint32_t d0Pin,
    GPIO_TypeDef *d1Port, uint32_t d1Pin,
    GPIO_TypeDef *d2Port, uint32_t d2Pin,
    GPIO_TypeDef *d3Port, uint32_t d3Pin,
    GPIO_TypeDef *d4Port, uint32_t d4Pin,
    GPIO_TypeDef *d5Port, uint32_t d5Pin,
    GPIO_TypeDef *d6Port, uint32_t d6Pin,
    GPIO_TypeDef *d7Port, uint32_t d7Pin);

#endif


/**
 * Get the clock frequency associated with the peripheral instance
 */
uint32_t stm32GetClockFrequency(void *instance);

/**
 * Get the ADC1 channel for the specified port / pin
 */
stm32_chip_adc1_channel_type stm32ADC1GetChannel(GPIO_TypeDef *port, uint32_t pin_mask);

/**
 * Internal: set the AF function for the selected peripheral on the selected pin, with GPIO_SPEED_FREQ_VERY_HIGH speed
 */
void stm32AfInit(const stm32_af_pin_list_type list[], int size, const void *instance, GPIO_TypeDef *port, uint32_t pin, uint32_t mode, uint32_t pull);

/**
 * Internal: set the AF function for the selected peripheral on the selected pin, with the specified GPIO speed
 */
void stm32AfInitSpeed(const stm32_af_pin_list_type list[], int size, const void *instance, GPIO_TypeDef *port, uint32_t pin, uint32_t mode, uint32_t pull, uint32_t speed);

/**
 * Internal: get the default pin for the given peripheral
 */
GPIO_TypeDef *stm32AfGetDefault(const stm32_af_pin_list_type list[], int size, const void *instance, uint32_t *pin);


#ifdef __cplusplus
}
#endif

#endif
