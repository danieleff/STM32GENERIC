/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.16 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUI_TOUCH_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/
#if __has_include("MW_STemWin.h")

#include "arduino.h"
#include "STemWin/inc/GUI.h"
#include "TOUCH/touch.h"
#include "LCD/BSP_tft_lcd.h"
#if __has_include("ott2001a.h")
  #include("ott2001a.h")
  #define  HAVE_OTT
#endif


void GUI_TOUCH_X_ActivateX(void) 
{
 // XPT2046_WriteCMD(0x90);
}


void GUI_TOUCH_X_ActivateY(void)
{
  //XPT2046_WriteCMD(0xd0);
}

int  GUI_TOUCH_X_MeasureX(void) 
{
	uint8_t buf[4];
	int32_t lost_x;
	static uint8_t mode; 
#ifdef  HAVE_OTT
	if(lcddev.id == 0X5510) //电容屏的触摸值获取
	{
		OTT2001A_RD_Reg(OTT_GSTID_REG,&mode,1);
		if(mode & 0X1F) //有触摸点按下时
		{
			OTT2001A_RD_Reg(0X0100,buf,4);//此处只读取电容触摸屏的x[0],y[0]
			tp_dev.x[0] = (((uint16_t)buf[2]<<8)+buf[3])*OTT_SCAL_X;
			lost_x = tp_dev.x[0];
			if(lost_x >479 ) lost_x =479;
			else if(lost_x < 0) lost_x=0;
		}else lost_x = 0;
		return lost_x;
	}else
#endif
		{
		return TP_Read_XOY(0XD0);  //CMD_RDX=0XD0
	}
}


int  GUI_TOUCH_X_MeasureY(void) 
{	
	uint8_t buf[4];
	int32_t lost_y;
	static uint8_t mode;
#ifdef  HAVE_OTT
	if(lcddev.id == 0X5510) //电容屏的触摸值获取
	{
		OTT2001A_RD_Reg(OTT_GSTID_REG,&mode,1); //获取触摸状态
		if(mode & 0X1F) //有触摸点按下时
		{
			OTT2001A_RD_Reg(0X0100,buf,4);//此处只读取电容触摸屏的x[0],y[0]
			tp_dev.y[0] = (((uint16_t)buf[0]<<8)+buf[1])*OTT_SCAL_Y;
			lost_y = tp_dev.y[0];
			if(lost_y > 799) lost_y = 799;
			else if(lost_y < 0) lost_y = 0;
		}else lost_y = 799;
		return lost_y;
	}else
#endif
		{
		return TP_Read_XOY(0X90);  //CMD_RDX=0XD0
	}
}

#endif  //STemWin
