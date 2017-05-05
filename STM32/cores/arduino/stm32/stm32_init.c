#include "stm32_def.h"

void init() {
  HAL_Init();
  
  SystemClock_Config();
  
  #ifdef STM32F1
  __HAL_RCC_AFIO_CLK_ENABLE();
  #endif
}

void SysTick_Handler(void) {
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}

void Error_Handler() {
    
}
