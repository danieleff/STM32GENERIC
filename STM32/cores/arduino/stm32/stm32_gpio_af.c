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

#include "stm32_gpio_af.h"

//add by huaweiwx@sina.com  2017.6.4
#ifdef STM32F1  
  #ifndef  AFIO_MAPR_SPI3_REMAP
    #define  __HAL_AFIO_REMAP_SPI3_DISABLE  AF_NO_REMAP
  #endif
#endif
#include CHIP_PERIPHERAL_INCLUDE

void stm32AfInit(const stm32_af_pin_list_type list[], int size, const void *instance, GPIO_TypeDef *port, uint32_t pin, uint32_t mode, uint32_t pull) {
    stm32AfInitSpeed(list, size, instance, port, pin, mode, pull, GPIO_SPEED_FREQ_VERY_HIGH);
}


GPIO_TypeDef *stm32AfGetDefault(const stm32_af_pin_list_type list[], int size, const void *instance, uint32_t *pin) {
    for(int i=0; i<size; i++) {
        if (instance == list[i].instance) {
            *pin = list[i].pin;
            return list[i].port;
        }
    }
    return NULL;
}

uint32_t stm32GetClockFrequency(void *instance) {
	for(size_t i=0; i<sizeof(chip_clock_freq_list) / sizeof(chip_clock_freq_list[0]); i++) {
		if (chip_clock_freq_list[i].instance == instance) {
			return (chip_clock_freq_list[i].clock_freq_func)();
		}
	}
	return 0;
}

stm32_chip_adc1_channel_type stm32ADC1GetChannel(GPIO_TypeDef *port, uint32_t pin_mask) {
    for(size_t i=0; i<sizeof(chip_adc1_channel) / sizeof(chip_adc1_channel[0]); i++) {
        if (chip_adc1_channel[i].port == port && chip_adc1_channel[i].pin_mask == pin_mask) {
            return chip_adc1_channel[i];
        }
    }

    stm32_chip_adc1_channel_type notFound = {0};
    return notFound;
}

void stm32AfUARTInit(const USART_TypeDef *instance,
    GPIO_TypeDef *rxPort, uint32_t rxPin,
    GPIO_TypeDef *txPort, uint32_t txPin) {

#ifdef STM32F1
    stm32AfInit(chip_af_usart_rx, sizeof(chip_af_usart_rx) / sizeof(chip_af_usart_rx[0]), instance, rxPort, rxPin, GPIO_MODE_AF_INPUT, GPIO_PULLUP);
#else
    stm32AfInit(chip_af_usart_rx, sizeof(chip_af_usart_rx) / sizeof(chip_af_usart_rx[0]), instance, rxPort, rxPin, GPIO_MODE_AF_PP, GPIO_PULLUP);
#endif
    stm32AfInit(chip_af_usart_tx, sizeof(chip_af_usart_tx) / sizeof(chip_af_usart_tx[0]), instance, txPort, txPin, GPIO_MODE_AF_PP, GPIO_PULLUP);

}

void stm32AfSPIInit(const SPI_TypeDef *instance,
    GPIO_TypeDef *mosiPort, uint32_t mosiPin,
    GPIO_TypeDef *misoPort, uint32_t misoPin,
	GPIO_TypeDef *sckPort, uint32_t sckPin) {

    stm32AfInit(chip_af_spi_mosi, sizeof(chip_af_spi_mosi) / sizeof(chip_af_spi_mosi[0]), instance, mosiPort, mosiPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_spi_miso, sizeof(chip_af_spi_miso) / sizeof(chip_af_spi_miso[0]), instance, misoPort, misoPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_spi_sck, sizeof(chip_af_spi_sck) / sizeof(chip_af_spi_sck[0]), instance, sckPort, sckPin, GPIO_MODE_AF_PP, GPIO_NOPULL);

}

SPI_TypeDef *stm32GetSPIInstance(GPIO_TypeDef *mosiPort, uint32_t mosiPin,
    GPIO_TypeDef *misoPort, uint32_t misoPin,
    GPIO_TypeDef *sckPort, uint32_t sckPin) {

    for (size_t i=0; i<sizeof(chip_af_spi_mosi) / sizeof(chip_af_spi_mosi[0]); i++) {
        if (chip_af_spi_mosi[i].port == mosiPort && chip_af_spi_mosi[i].pin == mosiPin) {

            for (size_t j=0; j<sizeof(chip_af_spi_miso) / sizeof(chip_af_spi_miso[0]); j++) {
                if (chip_af_spi_miso[j].port == misoPort && chip_af_spi_miso[j].pin == misoPin && chip_af_spi_mosi[i].instance == chip_af_spi_miso[j].instance) {

                    for (size_t k=0; k<sizeof(chip_af_spi_sck) / sizeof(chip_af_spi_sck[0]); k++) {
                       if (chip_af_spi_sck[k].port == sckPort && chip_af_spi_sck[k].pin == sckPin && chip_af_spi_mosi[i].instance == chip_af_spi_sck[k].instance) {

                           return chip_af_spi_mosi[i].instance;

                       }
                    }
                }
            }
        }
    }

    return NULL;
}

#ifdef STM32_CHIP_HAS_I2S

void stm32AfI2SInit(const SPI_TypeDef *instance,
    GPIO_TypeDef *sdPort, uint32_t sdPin,
    GPIO_TypeDef *wsPort, uint32_t wsPin,
    GPIO_TypeDef *ckPort, uint32_t ckPin) {

    stm32AfInit(chip_af_i2s_sd, sizeof(chip_af_i2s_sd) / sizeof(chip_af_i2s_sd[0]), instance, sdPort, sdPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_i2s_ws, sizeof(chip_af_i2s_ws) / sizeof(chip_af_i2s_ws[0]), instance, wsPort, wsPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_i2s_ck, sizeof(chip_af_i2s_ck) / sizeof(chip_af_i2s_ck[0]), instance, ckPort, ckPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
}

void stm32AfI2SInitWithMck(const SPI_TypeDef *instance,
    GPIO_TypeDef *sdPort, uint32_t sdPin,
    GPIO_TypeDef *wsPort, uint32_t wsPin,
    GPIO_TypeDef *ckPort, uint32_t ckPin,
    GPIO_TypeDef *mckPort, uint32_t mckPin) {

    stm32AfInit(chip_af_i2s_sd, sizeof(chip_af_i2s_sd) / sizeof(chip_af_i2s_sd[0]), instance, sdPort, sdPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_i2s_ws, sizeof(chip_af_i2s_ws) / sizeof(chip_af_i2s_ws[0]), instance, wsPort, wsPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_i2s_ck, sizeof(chip_af_i2s_ck) / sizeof(chip_af_i2s_ck[0]), instance, ckPort, ckPin, GPIO_MODE_AF_PP, GPIO_NOPULL);

#ifndef STM32_CHIP_NOT_MCK //add by huaweiwx@sina.com 2017.6.4	
    stm32AfInit(chip_af_i2s_mck, sizeof(chip_af_i2s_mck) / sizeof(chip_af_i2s_mck[0]), instance, mckPort, mckPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
#endif
}

#endif

void stm32AfI2CInit(const I2C_TypeDef *instance,
    GPIO_TypeDef *sdaPort, uint32_t sdaPin,
    GPIO_TypeDef *sclPort, uint32_t sclPin) {

    stm32AfInit(chip_af_i2c_scl, sizeof(chip_af_i2c_scl) / sizeof(chip_af_i2c_scl[0]), instance, sclPort, sclPin, GPIO_MODE_AF_OD, GPIO_PULLUP);
    stm32AfInit(chip_af_i2c_sda, sizeof(chip_af_i2c_sda) / sizeof(chip_af_i2c_sda[0]), instance, sdaPort, sdaPin, GPIO_MODE_AF_OD, GPIO_PULLUP);
}

#ifdef STM32_CHIP_HAS_SDIO

void stm32AfSDIO1BitInit(const SD_TypeDef *instance,
    GPIO_TypeDef *ckPort, uint32_t ckPin,
    GPIO_TypeDef *cmdPort, uint32_t cmdPin,
    GPIO_TypeDef *d0Port, uint32_t d0Pin
    ) {

    stm32AfInitSpeed(chip_af_sdio_ck, sizeof(chip_af_sdio_ck) / sizeof(chip_af_sdio_ck[0]), instance, ckPort, ckPin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
    stm32AfInitSpeed(chip_af_sdio_cmd, sizeof(chip_af_sdio_cmd) / sizeof(chip_af_sdio_cmd[0]), instance, cmdPort, cmdPin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
    stm32AfInitSpeed(chip_af_sdio_d0, sizeof(chip_af_sdio_d0) / sizeof(chip_af_sdio_d0[0]), instance, d0Port, d0Pin, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
}

void stm32AfSDIO4BitInit(const SD_TypeDef *instance,
    GPIO_TypeDef *ckPort, uint32_t ckPin,
    GPIO_TypeDef *cmdPort, uint32_t cmdPin,
    GPIO_TypeDef *d0Port, uint32_t d0Pin,
    GPIO_TypeDef *d1Port, uint32_t d1Pin,
    GPIO_TypeDef *d2Port, uint32_t d2Pin,
    GPIO_TypeDef *d3Port, uint32_t d3Pin
    ) {

    stm32AfInit(chip_af_sdio_ck, sizeof(chip_af_sdio_ck) / sizeof(chip_af_sdio_ck[0]), instance, ckPort, ckPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_cmd, sizeof(chip_af_sdio_cmd) / sizeof(chip_af_sdio_cmd[0]), instance, cmdPort, cmdPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d0, sizeof(chip_af_sdio_d0) / sizeof(chip_af_sdio_d0[0]), instance, d0Port, d0Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d1, sizeof(chip_af_sdio_d1) / sizeof(chip_af_sdio_d1[0]), instance, d1Port, d1Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d2, sizeof(chip_af_sdio_d2) / sizeof(chip_af_sdio_d2[0]), instance, d2Port, d2Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d3, sizeof(chip_af_sdio_d3) / sizeof(chip_af_sdio_d3[0]), instance, d3Port, d3Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
}

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
    GPIO_TypeDef *d7Port, uint32_t d7Pin
    ) {

    stm32AfInit(chip_af_sdio_ck, sizeof(chip_af_sdio_ck) / sizeof(chip_af_sdio_ck[0]), instance, ckPort, ckPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_cmd, sizeof(chip_af_sdio_cmd) / sizeof(chip_af_sdio_cmd[0]), instance, cmdPort, cmdPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d0, sizeof(chip_af_sdio_d0) / sizeof(chip_af_sdio_d0[0]), instance, d0Port, d0Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d1, sizeof(chip_af_sdio_d1) / sizeof(chip_af_sdio_d1[0]), instance, d1Port, d1Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d2, sizeof(chip_af_sdio_d2) / sizeof(chip_af_sdio_d2[0]), instance, d2Port, d2Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d3, sizeof(chip_af_sdio_d3) / sizeof(chip_af_sdio_d3[0]), instance, d3Port, d3Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d4, sizeof(chip_af_sdio_d4) / sizeof(chip_af_sdio_d4[0]), instance, d4Port, d4Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d5, sizeof(chip_af_sdio_d5) / sizeof(chip_af_sdio_d5[0]), instance, d5Port, d5Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d6, sizeof(chip_af_sdio_d6) / sizeof(chip_af_sdio_d6[0]), instance, d6Port, d6Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d7, sizeof(chip_af_sdio_d7) / sizeof(chip_af_sdio_d7[0]), instance, d7Port, d7Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
}

#endif
