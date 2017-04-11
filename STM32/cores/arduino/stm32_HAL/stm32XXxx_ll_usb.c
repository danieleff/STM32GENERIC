#include "stm32_build_defines.h"

#ifdef STM32F0
    
#endif
#ifdef STM32F1
    #include "stm32f1xx_ll_usb.c"
#endif
#ifdef STM32F2
    #include "stm32f2xx_ll_usb.c"
#endif
#ifdef STM32F3
    
#endif
#ifdef STM32F4
    #include "stm32f4xx_ll_usb.c"
#endif
#ifdef STM32F7
    #include "stm32f7xx_ll_usb.c"
#endif
#ifdef STM32L0
    
#endif
#ifdef STM32L1
    
#endif
#ifdef STM32L4
    #include "stm32l4xx_ll_usb.c"
#endif
