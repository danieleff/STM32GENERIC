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

alternate_callback stm32_pin_alternate(alternate_pin_type list[], int size, const void *instance, const GPIO_TypeDef *port, const uint32_t pin) {
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

void stm32_chip_UART_GPIO_init(const USART_TypeDef *instance, const GPIO_TypeDef *rxPort, const uint32_t rxPin, const GPIO_TypeDef *txPort, const uint32_t txPin) {
  GPIO_InitTypeDef GPIO_InitStruct;
  
  stm32_gpio_clock(rxPort);
  if (rxPort == NULL) {
      rxPort = stm32_alternate_default(alternate_usart_rx, sizeof(alternate_usart_rx) / sizeof(alternate_usart_rx[0]), instance, &rxPin);
  }
  GPIO_InitStruct.Pin = port_pin_list[rxPin].pin_mask;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  stm32_pin_alternate(alternate_usart_rx, sizeof(alternate_usart_rx) / sizeof(alternate_usart_rx[0]), instance, port_pin_list[rxPin].port, port_pin_list[rxPin].pin_mask)();
  
  stm32_gpio_clock(txPort);
  if (txPort == NULL) {
      txPort = stm32_alternate_default(alternate_usart_tx, sizeof(alternate_usart_tx) / sizeof(alternate_usart_tx[0]), instance, &txPin);
  }
  GPIO_InitStruct.Pin = port_pin_list[txPin].pin_mask;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  stm32_pin_alternate(alternate_usart_tx, sizeof(alternate_usart_tx) / sizeof(alternate_usart_tx[0]), instance, port_pin_list[txPin].port, port_pin_list[txPin].pin_mask)();
}

void stm32_chip_UART_GPIO_init_default(const USART_TypeDef *instance) {
    stm32_chip_UART_GPIO_init(instance, NULL, 0, NULL, 0);
}


#endif
