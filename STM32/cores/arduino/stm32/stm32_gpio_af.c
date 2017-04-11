#include "stm32_gpio_af.h"

#include CHIP_PERIPHERAL_INCLUDE

GPIO_TypeDef *stm32_af_default(alternate_pin_type list[], int size, const void *instance, uint32_t *pin) {
    for(int i=0; i<size; i++) {
        if (instance == list[i].instance) {
            *pin = list[i].pin;
            return list[i].port;
        }
    }
    return NULL;
}

void stm32_af_uart_init(const USART_TypeDef *instance, GPIO_TypeDef *rxPort, uint32_t rxPin, GPIO_TypeDef *txPort, uint32_t txPin) {

    stm32_af_init(alternate_usart_rx, sizeof(alternate_usart_rx) / sizeof(alternate_usart_rx[0]), instance, rxPort, rxPin, GPIO_MODE_AF_PP, GPIO_PULLUP);
    stm32_af_init(alternate_usart_tx, sizeof(alternate_usart_tx) / sizeof(alternate_usart_tx[0]), instance, txPort, txPin, GPIO_MODE_AF_PP, GPIO_PULLUP);

}
