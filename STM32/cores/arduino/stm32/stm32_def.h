#ifndef STM32_DEF_H
#define STM32_DEF_H

#include "stm32_build_defines.h"

#ifdef STM32F0
    #include "stm32f0xx.h"
    #include "stm32f0xx_hal.h"
#elif defined(STM32F1)
    #include "stm32f1xx.h"
    #include "stm32f1xx_hal.h"
#elif defined(STM32F2)
    #include "stm32f2xx.h"
    #include "stm32f2xx_hal.h"
#elif defined(STM32F3)
    #include "stm32f3xx.h"
    #include "stm32f3xx_hal.h"
#elif defined(STM32F4)
    #include "stm32f4xx.h"
    #include "stm32f4xx_hal.h"
#elif defined(STM32F7)
    #include "stm32f7xx.h"
    #include "stm32f7xx_hal.h"
#elif defined(STM32L0)
    #include "stm32l0xx.h"
    #include "stm32l0xx_hal.h"
#elif defined(STM32L1)
    #include "stm32l1xx.h"
    #include "stm32l1xx_hal.h"
#elif defined(STM32L4)
    #include "stm32l4xx.h"
    #include "stm32l4xx_hal.h"
#else
    #error "STM32XX is not defined in boards.txt"
#endif


#endif
