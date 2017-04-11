#if defined(STM32F0) || defined(STM32F2) || defined(STM32F3) || defined(STM32F4) || \
    defined(STM32F7) || defined(STM32L0) || defined(STM32L1) || defined(STM32L4)

#include "stm32_gpio_af.h"

int8_t stm32_af_get(const alternate_pin_type list[], int size, const void *instance, const GPIO_TypeDef *port, const uint32_t pin) {
    for(int i=0; i<size; i++) {
        if (instance == list[i].instance
            && port == list[i].port
            && pin == list[i].pin) {
                
            return list[i].alternate;
        }
    }
    return 0;
}

void stm32_af_init(const alternate_pin_type list[], int size, const void *instance, GPIO_TypeDef *port, uint32_t pin, uint32_t mode, uint32_t pull) {
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

#endif
