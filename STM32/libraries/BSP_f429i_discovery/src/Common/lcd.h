/**
  ******************************************************************************
  * @file    lcd.h
  * @author  MCD Application Team
  * @version V4.0.1
  * @date    21-July-2015
  * @brief   This file contains all the functions prototypes for the LCD driver.   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 
//modify by huaweiwx@sina.com 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H


#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

enum
{
	IC_5420		= 0x5420,
	IC_4001		= 0x4001,
	IC_61509 	= 0xB509,
	IC_8875 	= 0x0075,
};


typedef struct
{
	volatile uint16_t  REG;
	volatile uint16_t  RAM;
} LCD_IO_TypeDef;

//LCD chip
typedef struct  
{										    
	uint16_t 	id;				//LCD ID
	uint8_t  	dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t	wramcmd;		//开始写gram指令
	uint16_t  	setxcmd;		//设置x坐标指令
	uint16_t  	setycmd;		//设置y坐标指令
//    uint16_t    x,y;	        //当前x,y;
}LCD_DevTypeDef; 	  
  
typedef struct
{
  void     (*Init)(void);
  uint16_t (*ReadID)(void);
  void     (*DisplayOn)(void);
  void     (*DisplayOff)(void);
  void     (*SetCursor)(uint16_t Xpos, uint16_t Ypos);
  void     (*WritePixel)(uint16_t Xpos, uint16_t Ypos,uint16_t RGB_Code);
  uint16_t (*ReadPixel)(uint16_t Xpos, uint16_t Ypos);

  void     (*SetDisplayWindow)(uint16_t, uint16_t, uint16_t, uint16_t);
  void     (*DrawHLine)(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length);
  void     (*DrawVLine)(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length);
  
  uint16_t (*GetLcdPixelWidth)(void);
  uint16_t (*GetLcdPixelHeight)(void);
  void     (*DrawBitmap)(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);
  void     (*DrawRGBImage)(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata);

/* huawei add */
//  void     (*ClrScr)(uint16_t RGBCode);  
}LCD_DrvTypeDef;    
 
/* huawei add */
typedef struct
{
  void     (*pfWrite16_A0)(uint16_t _RegVal);
  void     (*pfWrite16_A1)(uint16_t Value);
  uint16_t (*pfRead16_A1)(void);  
  void     (*pfWriteM16_A1)(uint16_t *pdata,uint16_t size);
  void     (*pfReadM16_A1) (uint16_t *pdata,uint16_t size);
}LCD_IO_DrvTypeDef;    

extern uint16_t g_chipID;

#ifdef __cplusplus
}
#endif

#endif /* __LCD_H */

/**************(C) COPYRIGHT STMicroelectronics *****END OF FILE****/
