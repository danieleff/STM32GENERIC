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

#define RwReg uint32_t


#ifdef __cplusplus
extern "C"{
#endif

typedef struct {
    GPIO_TypeDef *port;
    uint32_t pin_mask;
} stm32_port_pin_type;

typedef uint32_t (*stm32_clock_freq_func)();

typedef struct {
    void *instance;
    stm32_clock_freq_func clock_freq_func;
} stm32_clock_freq_list_type;

extern const stm32_port_pin_type variant_pin_list[NUM_PINS];

/**
 * Start clock for the fedined port
 */
void stm32GpioClockEnable(GPIO_TypeDef *port);

/**
 * If PWM is used at least once, this method is set to the PWM disable function in stm32_PWM.c
 */
typedef void (*stm32_pwm_disable_callback_func)(GPIO_TypeDef *port, uint32_t pin);
extern stm32_pwm_disable_callback_func stm32_pwm_disable_callback;

inline void digitalWrite(uint8_t pin, uint8_t value) {
    if (pin >= sizeof(variant_pin_list) / sizeof(variant_pin_list[0])) {
        return;
    }
    
    stm32_port_pin_type port_pin = variant_pin_list[pin];
    
    HAL_GPIO_WritePin(port_pin.port, port_pin.pin_mask, value ? GPIO_PIN_SET : GPIO_PIN_RESET);
    
}


inline int digitalRead(uint8_t pin) {
    if (pin >= sizeof(variant_pin_list) / sizeof(variant_pin_list[0])) {
        return 0;
    }
    
    stm32_port_pin_type port_pin = variant_pin_list[pin];
    
    return HAL_GPIO_ReadPin(port_pin.port, port_pin.pin_mask);
    
}

#ifdef __cplusplus
}
#endif

#endif
