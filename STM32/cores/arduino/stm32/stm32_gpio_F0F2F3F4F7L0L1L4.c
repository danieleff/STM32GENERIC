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

int8_t stm32_get_alternate(alternate_pin_type list[], int size, const void *instance, const GPIO_TypeDef *port, const uint32_t pin) {
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
  GPIO_InitStruct.Pin = rxPin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = stm32_get_alternate(alternate_usart_rx, sizeof(alternate_usart_rx) / sizeof(alternate_usart_rx[0]), instance, rxPort, rxPin);
  HAL_GPIO_Init(rxPort, &GPIO_InitStruct);
  
  stm32_gpio_clock(txPort);
  if (txPort == NULL) {
      txPort = stm32_alternate_default(alternate_usart_tx, sizeof(alternate_usart_tx) / sizeof(alternate_usart_tx[0]), instance, &txPin);
  }
  GPIO_InitStruct.Pin = txPin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = stm32_get_alternate(alternate_usart_tx, sizeof(alternate_usart_tx) / sizeof(alternate_usart_tx[0]), instance, txPort, txPin);
  HAL_GPIO_Init(txPort, &GPIO_InitStruct);
}

void stm32_chip_UART_GPIO_init_default(const USART_TypeDef *instance) {
    stm32_chip_UART_GPIO_init(instance, NULL, 0, NULL, 0);
}


#endif
