#ifndef STM32_GPIO_H
#define STM32_GPIO_H

#include "stm32_def.h"

#include "variant.h"

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define INPUT_PULLDOWN 0x3

#ifndef GPIO_SPEED_FREQ_VERY_HIGH
    #define GPIO_SPEED_FREQ_VERY_HIGH GPIO_SPEED_FREQ_HIGH
#endif

#ifdef __cplusplus
extern "C"{
#endif

typedef struct {
    GPIO_TypeDef *port;
    uint32_t pin_mask;
} stm32_port_pin_type;

extern const stm32_port_pin_type port_pin_list[NUM_PINS];

void stm32_chip_UART_GPIO_init(const USART_TypeDef *instance, const GPIO_TypeDef *rxPort, const uint32_t rxPin, const GPIO_TypeDef *txPort, const uint32_t txPin);
void stm32_chip_UART_GPIO_init_default(const USART_TypeDef *instance);

void stm32_gpio_clock(GPIO_TypeDef *port);

inline void digitalWrite(uint8_t pin, uint8_t value) {
    if (pin < 0 || pin >= sizeof(port_pin_list) / sizeof(port_pin_list[0])) {
        return;
    }
    
    stm32_port_pin_type port_pin = port_pin_list[pin];
    
    HAL_GPIO_WritePin(port_pin.port, port_pin.pin_mask, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
    
}


inline int digitalRead(uint8_t pin) {
    if (pin < 0 || pin >= sizeof(port_pin_list) / sizeof(port_pin_list[0])) {
        return 0;
    }
    
    stm32_port_pin_type port_pin = port_pin_list[pin];
    
    return HAL_GPIO_ReadPin(port_pin.port, port_pin.pin_mask);
    
}

#ifdef __cplusplus
}
#endif

#endif
