#include "stm32_gpio.h"

#define PIN(a, b) { GPIO##a , GPIO_PIN_##b }
const stm32_port_pin_type variant_pin_list[] = {
  PIN_LIST
};
#undef PIN
