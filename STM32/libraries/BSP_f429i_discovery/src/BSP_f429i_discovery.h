//BSP for arduino  huaweiwx@sina.com 2017.06.18
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_eeprom.h"
#include "stm32f429i_discovery_sdram.h"
#include "stm32f429i_discovery_ts.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_gyroscope.h"


#ifdef OS_SUPPORT
#	if __has_include("FreeRTOS_STM.h")
#		if (OS_SUPPORT !=1)
#   		warning "With FreeRTOS, must select STemWin lib: STemWin_OS on menu->tools->OS & Extern lib!"    		
#		endif
#	else
#		if (OS_SUPPORT ==1)
#   		warning "With unused FreeRTOS, must select STemWin lib: STemWin on menu->tools->OS & Extern lib!"    		
#		endif
#   endif
#else
#	if __has_include("MW_STemWin.h")
#   	warning "use STemWin, must select a STemWin lib on menu->tools->OS & Extern lib!"    		
#   endif
#endif	
	