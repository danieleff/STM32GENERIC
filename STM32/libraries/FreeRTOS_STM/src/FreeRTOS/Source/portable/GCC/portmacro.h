#if defined(STM32F0)||defined(STM32L0)
	#include "ARM_CM0/portmacro.h"
#elif defined(STM32F1)||defined(STM32L1)||defined(STM32F2)
	#include "ARM_CM3/portmacro.h"
#elif defined(STM32F4)||defined(STM32L4)||defined(STM32F3)
	#include "ARM_CM4F/portmacro.h"
#elif defined(STM32F7)
	#include "ARM_CM7/portmacro.h"
#else
	#error "Unknown ARM Cortex version"
#endif
