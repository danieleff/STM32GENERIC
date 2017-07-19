#if defined(STM32F0)||defined(STM32L0)
//__CM0_CMSIS_VERSION add by huaweiwx@sina.com 2017.6.13 un test
#include "portmacro_CM0.h"

#elif defined(STM32F1)||defined(STM32L1)||defined(STM32F2)

#include "portmacro_CM3.h"

#elif defined(STM32F3)||defined(STM32F4)||defined(STM32L4)

#include "portmacro_CM4F.h"

#elif defined(STM32F7)

#include "portmacro_CM7.h"

#else
    
#error "Unknown ARM Cortex version"

#endif
