/**
  ******************************************************************************
    @file    stm32f4xx_it.c
    @author  MCD Application Team
    @version V1.3.6
    @date    04-November-2016
    @brief   Main Interrupt Service Routines.
  ******************************************************************************
    @attention

    <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>

    Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
    You may not use this file except in compliance with the License.
    You may obtain a copy of the License at:

           http://www.st.com/software_license_agreement_liberty_v2

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#if !(__has_include("FreeRTOS.h"))


#include "stm32_def.h"  //for use st hal bsp lib huaweiwx@sina.com 2017.5

#include "STemWin/inc/GUI.h"
#include "STemWin/inc/WM.h"

/*
/**
* @brief  This function handles SysTick Handler.
* @param  None
* @retval None
*/

extern volatile GUI_TIMER_TIME  OS_TimeMS;

void HAL_SYSTICK_Callback (void)
{
  OS_TimeMS++;
}

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


