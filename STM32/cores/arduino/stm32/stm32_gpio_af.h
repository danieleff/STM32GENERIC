/**
 * Function to set up GPIO alternate functions for pins.
 *
 * If a port parameter is NULL, the default port/pin will be used.
 */
#ifndef STM32_GPIO_AF_H
#define STM32_GPIO_AF_H

#include "stm32_gpio.h"

#ifdef STM32F1

typedef void (*alternate_callback)();

typedef struct {
    void *instance;
    GPIO_TypeDef *port;
    uint32_t pin;
    alternate_callback alternate;
} alternate_pin_type;

#else

typedef struct {
    void *instance;
    GPIO_TypeDef *port;
    uint32_t pin;
    uint8_t alternate;
} alternate_pin_type;

#endif


#ifdef __cplusplus
extern "C"{
#endif

void stm32_af_uart_init(const USART_TypeDef *instance,
    GPIO_TypeDef *rxPort, uint32_t rxPin, 
    GPIO_TypeDef *txPort, uint32_t txPin);

void stm32_af_spi_init(const SPI_TypeDef *instance,
    GPIO_TypeDef *mosiPort, uint32_t mosiPin,
    GPIO_TypeDef *misoPort, uint32_t misoPin,
	GPIO_TypeDef *sckPort, uint32_t sckPin);

/**
 * Internal: set the AF function for the selected peripheral on the selected pin
 */
void stm32_af_init(const alternate_pin_type list[], int size, const void *instance, GPIO_TypeDef *port, uint32_t pin, uint32_t mode, uint32_t pull);

/**
 * Internal: get the default pin for the given peripheral
 */
GPIO_TypeDef *stm32_af_default(alternate_pin_type list[], int size, const void *instance, uint32_t *pin);


#ifdef __cplusplus
}
#endif

#endif
