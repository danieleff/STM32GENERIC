/*
  Copyright (c) 2017 Daniel Fekete

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "stm32_def.h"

extern void SystemClock_Config(void);

void init() {
  HAL_Init();
  
  SystemClock_Config();
  
#ifdef STM32F1
  __HAL_RCC_AFIO_CLK_ENABLE();
#endif
}

void SysTick_Handler(void) {
#if __has_include("FreeRTOS.h")  //huawei (huaweiwx@sina.com)
  osSystickHandler();	
#else	
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
#endif
}

void Error_Handler() {
    
}
