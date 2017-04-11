#if defined(STM32F0) || defined(STM32F2) || defined(STM32F3) || defined(STM32F4) || \
    defined(STM32F7) || defined(STM32L0) || defined(STM32L1) || defined(STM32L4)

#include "stm32_gpio.h"

typedef struct {
    void *instance;
    GPIO_TypeDef *port;
    uint32_t pin;
    uint8_t alternate;
} alternate_pin_type;

#include CHIP_PERIPHERAL_INCLUDE

int8_t stm32_af_get(alternate_pin_type list[], int size, const void *instance, const GPIO_TypeDef *port, const uint32_t pin) {
    for(int i=0; i<size; i++) {
        if (instance == list[i].instance
            && port == list[i].port
            && pin == list[i].pin) {
                
            return list[i].alternate;
        }
    }
    return 0;
}

GPIO_TypeDef *stm32_af_default(alternate_pin_type list[], int size, const void *instance, uint32_t *pin) {
    for(int i=0; i<size; i++) {
        if (instance == list[i].instance) {
            *pin = list[i].pin;
            return list[i].port;
        }
    }
    return NULL;
}

void stm32_af_init(alternate_pin_type list[], int size, void *instance, GPIO_TypeDef *port, uint32_t *pin, uint32_t mode, uint32_t pull) {
    if (port == NULL) {
        port = stm32_af_default(list, size, instance, &pin);
    }
    stm32_gpio_clock(port);
    
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Pull = pull;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = stm32_af_get(list, size, instance, port, pin);
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}

void stm32_af_uart_init(const USART_TypeDef *instance, GPIO_TypeDef *rxPort, uint32_t rxPin, GPIO_TypeDef *txPort, uint32_t txPin) {
    
    stm32_af_init(alternate_usart_rx, sizeof(alternate_usart_rx) / sizeof(alternate_usart_rx[0]), instance, rxPort, rxPin, GPIO_MODE_AF_PP, GPIO_PULLUP);
    stm32_af_init(alternate_usart_tx, sizeof(alternate_usart_tx) / sizeof(alternate_usart_tx[0]), instance, txPort, txPin, GPIO_MODE_AF_PP, GPIO_PULLUP);
    
}

#endif
