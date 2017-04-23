#if defined(__CM0PLUS_CMSIS_VERSION)

#include "portmacro_CM0.h"

#elif defined(__CM3_CMSIS_VERSION)

#include "portmacro_CM3.h"

#elif defined(__CM4_CMSIS_VERSION)

#include "portmacro_CM4F.h"

#elif defined(__CM7_CMSIS_VERSION)

#include "portmacro_CM7.h"

#else
    
#error "Unknown ARM Cortex version"

#endif
