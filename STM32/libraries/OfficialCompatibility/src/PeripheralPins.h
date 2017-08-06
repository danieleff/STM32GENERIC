#define NC 0xFF

#include "Arduino.h"

typedef uint8_t PinName;

typedef struct {
    PinName pin;
    uint8_t mode;
    uint8_t pupd;
    uint8_t afnum;
} PinMap;

extern const PinMap PinMap_Ethernet[];

inline PinName pin_pinName(const PinMap* map) {return map->pin;}
const PinMap *pinmap_function(PinName pin, const PinMap* map);


#define STM_PORT(A) variant_pin_list[A].port
#define STM_GPIO_PIN(A) variant_pin_list[A].pin_mask

#define STM_PIN_MODE(A) A->mode
#define STM_PIN_PUPD(A) A->pupd
#define STM_PIN_AFNUM(A) A->afnum

inline GPIO_TypeDef *set_GPIO_Port_Clock(GPIO_TypeDef *port) {
    stm32GpioClockEnable(port);
    return port;
}
