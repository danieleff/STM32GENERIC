/**
 * Function to set up GPIO alternate functions for pins.
 *
 * If a port parameter is NULL, the default port/pin will be used.
 */
#ifndef STM32_GPIO_AF_H
#define STM32_GPIO_AF_H

#include "stm32_gpio.h"

#ifdef __cplusplus
extern "C"{
#endif

void stm32_af_uart_init(const USART_TypeDef *instance,
    GPIO_TypeDef *rxPort, uint32_t rxPin, 
    GPIO_TypeDef *txPort, uint32_t txPin);

#ifdef __cplusplus
}
#endif

#endif
