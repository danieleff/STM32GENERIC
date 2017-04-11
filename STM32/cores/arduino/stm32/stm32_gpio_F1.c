#if defined(STM32F1)

#include "stm32_gpio.h"

typedef void (*alternate_callback)();

typedef struct {
    void *instance;
    GPIO_TypeDef *port;
    uint32_t pin;
    alternate_callback alternate;
} alternate_pin_type;

#include CHIP_PERIPHERAL_INCLUDE

alternate_callback stm32_alternate_get(alternate_pin_type list[], int size, const void *instance, const GPIO_TypeDef *port, const uint32_t pin) {
    for(int i=0; i<size; i++) {
        if (instance == list[i].instance
            && port == list[i].port
            && pin == list[i].pin) {
                
            return list[i].alternate;
        }
    }
    return 0;
}

GPIO_TypeDef *stm32_alternate_default(alternate_pin_type list[], int size, const void *instance, uint32_t *pin) {
    for(int i=0; i<size; i++) {
        if (instance == list[i].instance) {
            *pin = list[i].pin;
            return list[i].port;
        }
    }
    return NULL;
}

void stm32_alternate_init(alternate_pin_type list[], int size, void *instance, GPIO_TypeDef *port, uint32_t *pin, uint32_t mode, uint32_t pull) {
    if (port == NULL) {
        port = stm32_alternate_default(list, size, instance, &pin);
    }
    stm32_gpio_clock(port);
    
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Pull = pull;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
    
    stm32_alternate_get(list, size, instance, port, pin)();
}

void stm32_alternate_uart_init(const USART_TypeDef *instance, GPIO_TypeDef *rxPort, uint32_t rxPin, GPIO_TypeDef *txPort, uint32_t txPin) {

    stm32_alternate_init(alternate_usart_rx, sizeof(alternate_usart_rx) / sizeof(alternate_usart_rx[0]), instance, rxPort, rxPin, GPIO_MODE_AF_PP, GPIO_PULLUP);
    stm32_alternate_init(alternate_usart_tx, sizeof(alternate_usart_tx) / sizeof(alternate_usart_tx[0]), instance, txPort, txPin, GPIO_MODE_AF_PP, GPIO_PULLUP);

}


#endif
