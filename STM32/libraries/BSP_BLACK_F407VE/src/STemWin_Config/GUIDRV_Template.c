/*********************************************************************
*          Portions COPYRIGHT 2013 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.22 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIDRV_Template.c
Purpose     : Template driver, could be used as starting point for new
              simple display drivers supporting only one color depth.
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
#if __has_include("MW_STemWin.h")
#include "arduino.h"
#include "STemWin/inc/LCD_Private.h"
#include "STemWin/inc/GUI_Private.h"
#include "STemWin/inc/LCD_ConfDefaults.h"
#include "LCD/BSP_tft_lcd.h"
#include "bsp.h"
//定义LCD的命令和数据位.
uint32_t UCGUI_LCD_CMD  = 0X6007FFFE; //地址为0X6007FFFE;  0111 1110  FSMC_A18为0 
uint32_t UCGUI_LCD_DATA = 0X60080000; //地址为0X60080000;  1000 0000  FSMC_A18为1

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/*********************************************************************
*
*       Macros for MIRROR_, SWAP_ and LUT_
*/
#if (!defined (LCD_LUT_COM) && !defined(LCD_LUT_SEG))
  #if   (!LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) x
    #define LOG2PHYS_Y(x, y) y
  #elif (!LCD_MIRROR_X && !LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) y
    #define LOG2PHYS_Y(x, y) x
  #elif (!LCD_MIRROR_X &&  LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) x
    #define LOG2PHYS_Y(x, y) LCD_YSIZE - 1 - (y)
  #elif (!LCD_MIRROR_X &&  LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) y
    #define LOG2PHYS_Y(x, y) LCD_XSIZE - 1 - (x)
  #elif ( LCD_MIRROR_X && !LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_XSIZE - 1 - (x)
    #define LOG2PHYS_Y(x, y) y
  #elif ( LCD_MIRROR_X && !LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_YSIZE - 1 - (y)
    #define LOG2PHYS_Y(x, y) x
  #elif ( LCD_MIRROR_X &&  LCD_MIRROR_Y && !LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_XSIZE - 1 - (x)
    #define LOG2PHYS_Y(x, y) LCD_YSIZE - 1 - (y)
  #elif ( LCD_MIRROR_X &&  LCD_MIRROR_Y &&  LCD_SWAP_XY) 
    #define LOG2PHYS_X(x, y) LCD_YSIZE - 1 - (y)
    #define LOG2PHYS_Y(x, y) LCD_XSIZE - 1 - (x)
  #endif
#else
  #if   ( defined (LCD_LUT_COM) && !defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) x
    #define LOG2PHYS_Y(x, y) LCD__aLine2Com0[y]
  #elif (!defined (LCD_LUT_COM) &&  defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) LCD__aCol2Seg0[x]
    #define LOG2PHYS_Y(x, y) y
  #elif ( defined (LCD_LUT_COM) &&  defined(LCD_LUT_SEG))
    #define LOG2PHYS_X(x, y) LCD__aCol2Seg0[x]
    #define LOG2PHYS_Y(x, y) LCD__aLine2Com0[y]
  #endif
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  uint32_t VRAMAddr;
  int xSize, ySize;
  int vxSize, vySize;
  int vxSizePhys;
  int BitsPerPixel;
} DRIVER_CONTEXT_TEMPLATE;

/*********************************************************************
*
*       Static functions
*
**********************************************************************
*/
/*********************************************************************
 打点函数
*/
static void _SetPixelIndex(GUI_DEVICE * pDevice, int x, int y, int PixelIndex) {
 
	if(lcddev.id == 0x9341 || lcddev.id == 0x5310)
	{
		//设置X坐标
		TFTLCD->REG  = lcddev.setxcmd;
		TFTLCD->RAM = (x>>8); 
		TFTLCD->RAM = (x&0XFF);
		
		//设置y坐标
		TFTLCD->REG  = lcddev.setycmd;
		TFTLCD->RAM = (y>>8); 
		TFTLCD->RAM = (y&0XFF);
	}else if(lcddev.id == 0X6804)
	{
		if(lcddev.id == 1)x=lcddev.width-1-x; //横屏时处理
		//设置X坐标
		TFTLCD->REG  = lcddev.setxcmd;
		TFTLCD->RAM = (x>>8); 
		TFTLCD->RAM = (x&0XFF);
		
		//设置y坐标
		TFTLCD->REG  = lcddev.setycmd;
		TFTLCD->RAM = (y>>8); 
		TFTLCD->RAM = (y&0XFF);
	}else if(lcddev.id == 0X5510) 
	{
		TFTLCD->REG  = lcddev.setxcmd; //设置X坐标高八位
		TFTLCD->RAM = (x>>8); //x地址地高八位
		TFTLCD->REG  = lcddev.setxcmd+1; //设置X坐标低八位
		TFTLCD->RAM = (x&0XFF);//x地址低八位
		
		TFTLCD->REG  = lcddev.setycmd; //设置y坐标高八位
		TFTLCD->RAM = (y>>8); //y地址地高八位
		TFTLCD->REG  = lcddev.setycmd+1; //设置y坐标低八位
		TFTLCD->RAM = (y&0XFF);//y地址低八位
	}else if(lcddev.id == 0X55420)//其他屏幕  0x5420
	{
		if(lcddev.id == 0) x=lcddev.width-1-x;  //缺省横屏
		TFTLCD->REG  = lcddev.setxcmd; 
		TFTLCD->RAM = x; 
		TFTLCD->REG  = lcddev.setycmd; 
		TFTLCD->RAM = y; 
	}else             //其他屏幕
	{
		if(lcddev.id == 1)x=lcddev.width-1-x;  //横屏其实就是调转X,Y坐标
		TFTLCD->REG  = lcddev.setxcmd; 
		TFTLCD->RAM = x; 
		TFTLCD->REG  = lcddev.setycmd; 
		TFTLCD->RAM = y; 
	}
	TFTLCD->REG  = lcddev.wramcmd; //写颜色值
	TFTLCD->RAM = PixelIndex;//向LCD的gram写入颜色值
}


/*********************************************************************
*
	读点函数
*/
static unsigned int _GetPixelIndex(GUI_DEVICE * pDevice, int x, int y) {
	unsigned int PixelIndex;
    #if (LCD_MIRROR_X == 1) || (LCD_MIRROR_Y == 1) || (LCD_SWAP_XY == 1)
      int xPhys, yPhys;

      xPhys = LOG2PHYS_X(x, y);
      yPhys = LOG2PHYS_Y(x, y);
    #else
      #define xPhys x
      #define yPhys y
    #endif
    GUI_USE_PARA(pDevice);
    GUI_USE_PARA(x);
    GUI_USE_PARA(y);
    {
			PixelIndex = LCD_ReadPoint(x,y);
    }
    #if (LCD_MIRROR_X == 0) && (LCD_MIRROR_Y == 0) && (LCD_SWAP_XY == 0)
      #undef xPhys
      #undef yPhys
    #endif
  return PixelIndex;
	
}
/*********************************************************************
*
*       _XorPixel
*/
static void _XorPixel(GUI_DEVICE * pDevice, int x, int y) {
  LCD_PIXELINDEX PixelIndex;
  LCD_PIXELINDEX IndexMask;

  PixelIndex = _GetPixelIndex(pDevice, x, y);
  IndexMask  = pDevice->pColorConvAPI->pfGetIndexMask();
  _SetPixelIndex(pDevice, x, y, PixelIndex ^ IndexMask);
}

/*********************************************************************
*
*       _FillRect
*/
static void _FillRect(GUI_DEVICE * pDevice, int x0, int y0, int x1, int y1) {
	
	uint32_t n;
	if(lcddev.id == 0X9341 || lcddev.id == 0X5310 ||lcddev.id == 0X6804) //6804横屏不支持！！！！
	{
		//设置窗口大小,X轴坐标的起始和终止坐标
		TFTLCD->REG  = lcddev.setxcmd; 
		TFTLCD->RAM = (x0>>8); 
		TFTLCD->RAM = (x0&0XFF);
		TFTLCD->RAM = (x1>>8);  
		TFTLCD->RAM = (x1&0XFF);
		
		//设置窗口大小,Y轴的起始和终止坐标
		TFTLCD->REG  = lcddev.setycmd; 
		TFTLCD->RAM = (y0>>8);
		TFTLCD->RAM = (y0&0XFF);
		TFTLCD->RAM = (y1>>8); 
		TFTLCD->RAM = (y1&0XFF);
		
		//设置开始坐标
		TFTLCD->REG  = lcddev.setxcmd; 
		TFTLCD->RAM = (x0>>8); 
		TFTLCD->RAM = (x0&0XFF);
		TFTLCD->REG  = lcddev.setycmd; 
		TFTLCD->RAM = (y0>>8);
		TFTLCD->RAM = (y0&0XFF);
		
		n=(uint32_t)(y1-y0+1)*(x1-x0+1);
		TFTLCD->REG  = lcddev.wramcmd;  //写入颜色值
		while(n--)
		{
			TFTLCD->RAM = LCD_COLORINDEX;//向LCD的gram写入颜色值
		}
		
		//恢复窗口大小
		TFTLCD->REG  = lcddev.setxcmd; 
		TFTLCD->RAM = (0X00>>8);  
		TFTLCD->RAM = (0X00&0XFF);
		TFTLCD->RAM = (lcddev.width>>8);  
		TFTLCD->RAM = (lcddev.width&0XFF);
		
		//恢复窗口大小
		TFTLCD->REG  = lcddev.setycmd; 
		TFTLCD->RAM = (0X00>>8);
		TFTLCD->RAM = (0X00&0XFF);
		TFTLCD->RAM = (lcddev.height>>8); 
		TFTLCD->RAM = (lcddev.height&0XFF);
		
	}else if(lcddev.id == 0X5510)
	{
		//设置窗口大小,X轴坐标的起始和终止坐标
		TFTLCD->REG  = lcddev.setxcmd; 
		TFTLCD->RAM = (x0>>8); 
		TFTLCD->REG  = lcddev.setxcmd+1; 
		TFTLCD->RAM = (x0&0XFF);
		TFTLCD->REG  = lcddev.setxcmd+2; 
		TFTLCD->RAM = (x1>>8); 
		TFTLCD->REG  = lcddev.setxcmd+3; 
		TFTLCD->RAM = (x1&0XFF);
		
		//设置窗口大小,Y轴的起始和终止坐标
		TFTLCD->REG  = lcddev.setycmd; 
		TFTLCD->RAM = (y0>>8);
		TFTLCD->REG  = lcddev.setycmd+1; 
		TFTLCD->RAM = (y0&0XFF);
		TFTLCD->REG  = lcddev.setycmd+2; 
		TFTLCD->RAM = (y1>>8); 
		TFTLCD->REG  = lcddev.setycmd+3;
		TFTLCD->RAM = (y1&0XFF);
			
		//设置开始坐标
		TFTLCD->REG  = lcddev.setxcmd; 
		TFTLCD->RAM = (x0>>8); 
		TFTLCD->REG  = lcddev.setxcmd+1; 
		TFTLCD->RAM = (x0&0XFF);
		TFTLCD->REG  = lcddev.setycmd; 
		TFTLCD->RAM = (y0>>8);
		TFTLCD->REG  = lcddev.setycmd+1; 
		TFTLCD->RAM = (y0&0XFF);
		
		n=(uint32_t)(y1-y0+1)*(x1-x0+1);
		TFTLCD->REG  = lcddev.wramcmd;  //写入颜色值
		while(n--)
		{
			TFTLCD->RAM = LCD_COLORINDEX;//向LCD的gram写入颜色值
		}
		
		//恢复窗口大小
		TFTLCD->REG  = lcddev.setxcmd; 
		TFTLCD->RAM = (0X00>>8);  
		TFTLCD->REG  = lcddev.setxcmd+1; 
		TFTLCD->RAM = (0X00&0XFF);
		TFTLCD->REG  = lcddev.setxcmd+2; 
		TFTLCD->RAM = (lcddev.width>>8);  //0X31F = 800
		TFTLCD->REG  = lcddev.setxcmd+3; 
		TFTLCD->RAM = (lcddev.width&0XFF);
		
		//恢复窗口大小
		TFTLCD->REG  = lcddev.setycmd; 
		TFTLCD->RAM = (0X00>>8);
		TFTLCD->REG  = lcddev.setycmd+1; 
		TFTLCD->RAM = (0X00&0XFF);
		TFTLCD->REG  = lcddev.setycmd+2; 
		TFTLCD->RAM = (lcddev.height>>8);  //0X1DF = 480
		TFTLCD->REG  = lcddev.setycmd+3;
		TFTLCD->RAM = (lcddev.height&0XFF);
	}else if(lcddev.id == 0X5420)
	{
		//设置窗口大小,X轴坐标的起始和终止坐标
		TFTLCD->REG  = 0x0212; 
		TFTLCD->RAM = x0; 
		TFTLCD->REG  = 0x0213; 
		TFTLCD->RAM = x1;
	
		//设置窗口大小,Y轴的起始和终止坐标
		TFTLCD->REG  = 0x210; 
		TFTLCD->RAM = y0;
		TFTLCD->REG  = 0x211; 
		TFTLCD->RAM = y1; 
			
		//设置开始坐标
		TFTLCD->REG  = lcddev.setxcmd; 
		TFTLCD->RAM = x0; 
		TFTLCD->REG  = lcddev.setycmd; 
		TFTLCD->RAM = y0;
		
		n=(uint32_t)(y1-y0+1)*(x1-x0+1);
		TFTLCD->REG  = lcddev.wramcmd;  //写入颜色值
		while(n--)
		{
			TFTLCD->RAM = LCD_COLORINDEX;//向LCD的gram写入颜色值
		}
		
		//恢复窗口大小
		TFTLCD->REG  = 0x0212; 
		TFTLCD->RAM = 0; 
		TFTLCD->REG  = 0x0213; 
		TFTLCD->RAM = lcddev.width;
	
		//设置窗口大小,Y轴的起始和终止坐标
		TFTLCD->REG  = 0x210;
		TFTLCD->RAM = 0;
		TFTLCD->REG  = 0x211; 
		TFTLCD->RAM = lcddev.height; 
	}

}


/*********************************************************************
*
*       _DrawHLine
*/
static void _DrawHLine(GUI_DEVICE * pDevice, int x0, int y, int x1) {
  _FillRect(pDevice, x0, y, x1, y);
}

/*********************************************************************
*
*       _DrawVLine, not optimized
*/
static void _DrawVLine(GUI_DEVICE * pDevice, int x, int y0, int y1) {
  _FillRect(pDevice, x, y0, x, y1);
}

/*********************************************************************
*
*       Draw Bitmap 1 BPP
*/
static void _DrawBitLine1BPP(GUI_DEVICE * pDevice, int x, int y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX IndexMask, Index0, Index1, Pixel;

  Index0 = *(pTrans + 0);
  Index1 = *(pTrans + 1);
  x += Diff;
  switch (GUI_pContext->DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
  case 0:
    do {
      _SetPixelIndex(pDevice, x++, y, (*p & (0x80 >> Diff)) ? Index1 : Index0);
      if (++Diff == 8) {
        Diff = 0;
        p++;
      }
    } while (--xsize);
    break;
  case LCD_DRAWMODE_TRANS:
    do {
      if (*p & (0x80 >> Diff))
        _SetPixelIndex(pDevice, x, y, Index1);
      x++;
      if (++Diff == 8) {
        Diff = 0;
        p++;
      }
    } while (--xsize);
    break;
  case LCD_DRAWMODE_XOR | LCD_DRAWMODE_TRANS:
  case LCD_DRAWMODE_XOR:
    IndexMask = pDevice->pColorConvAPI->pfGetIndexMask();
    do {
      if (*p & (0x80 >> Diff)) {
        Pixel = _GetPixelIndex(pDevice, x, y);
        _SetPixelIndex(pDevice, x, y, Pixel ^ IndexMask);
      }
      x++;
      if (++Diff == 8) {
        Diff = 0;
        p++;
      }
    } while (--xsize);
    break;
  }
}

/*********************************************************************
*
*       Draw Bitmap 2 BPP
*/
static void  _DrawBitLine2BPP(GUI_DEVICE * pDevice, int x, int y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixels, PixelIndex;
  int CurrentPixel, Shift, Index;

  Pixels = *p;
  CurrentPixel = Diff;
  x += Diff;
  switch (GUI_pContext->DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
  case 0:
    if (pTrans) {
      do {
        Shift = (3 - CurrentPixel) << 1;
        Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
        PixelIndex = *(pTrans + Index);
        _SetPixelIndex(pDevice, x++, y, PixelIndex);
        if (++CurrentPixel == 4) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    } else {
      do {
        Shift = (3 - CurrentPixel) << 1;
        Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
        _SetPixelIndex(pDevice, x++, y, Index);
        if (++CurrentPixel == 4) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    }
    break;
  case LCD_DRAWMODE_TRANS:
    if (pTrans) {
      do {
        Shift = (3 - CurrentPixel) << 1;
        Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
        if (Index) {
          PixelIndex = *(pTrans + Index);
          _SetPixelIndex(pDevice, x, y, PixelIndex);
        }
        x++;
        if (++CurrentPixel == 4) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    } else {
      do {
        Shift = (3 - CurrentPixel) << 1;
        Index = (Pixels & (0xC0 >> (6 - Shift))) >> Shift;
        if (Index) {
          _SetPixelIndex(pDevice, x, y, Index);
        }
        x++;
        if (++CurrentPixel == 4) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    }
    break;
  }
}

/*********************************************************************
*
*       Draw Bitmap 4 BPP
*/
static void  _DrawBitLine4BPP(GUI_DEVICE * pDevice, int x, int y, U8 const GUI_UNI_PTR * p, int Diff, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixels, PixelIndex;
  int CurrentPixel, Shift, Index;

  Pixels = *p;
  CurrentPixel = Diff;
  x += Diff;
  switch (GUI_pContext->DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
  case 0:
    if (pTrans) {
      do {
        Shift = (1 - CurrentPixel) << 2;
        Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        PixelIndex = *(pTrans + Index);
        _SetPixelIndex(pDevice, x++, y, PixelIndex);
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    } else {
      do {
        Shift = (1 - CurrentPixel) << 2;
        Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        _SetPixelIndex(pDevice, x++, y, Index);
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    }
    break;
  case LCD_DRAWMODE_TRANS:
    if (pTrans) {
      do {
        Shift = (1 - CurrentPixel) << 2;
        Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        if (Index) {
          PixelIndex = *(pTrans + Index);
          _SetPixelIndex(pDevice, x, y, PixelIndex);
        }
        x++;
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    } else {
      do {
        Shift = (1 - CurrentPixel) << 2;
        Index = (Pixels & (0xF0 >> (4 - Shift))) >> Shift;
        if (Index) {
          _SetPixelIndex(pDevice, x, y, Index);
        }
        x++;
        if (++CurrentPixel == 2) {
          CurrentPixel = 0;
          Pixels = *(++p);
        }
      } while (--xsize);
    }
    break;
  }
}

/*********************************************************************
*
*       Draw Bitmap 8 BPP
*/
static void  _DrawBitLine8BPP(GUI_DEVICE * pDevice, int x, int y, U8 const GUI_UNI_PTR * p, int xsize, const LCD_PIXELINDEX * pTrans) {
  LCD_PIXELINDEX Pixel;

  switch (GUI_pContext->DrawMode & (LCD_DRAWMODE_TRANS | LCD_DRAWMODE_XOR)) {
  case 0:
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        Pixel = *p;
        _SetPixelIndex(pDevice, x, y, *(pTrans + Pixel));
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        _SetPixelIndex(pDevice, x, y, *p);
      }
    }
    break;
  case LCD_DRAWMODE_TRANS:
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        Pixel = *p;
        if (Pixel) {
          _SetPixelIndex(pDevice, x, y, *(pTrans + Pixel));
        }
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        Pixel = *p;
        if (Pixel) {
          _SetPixelIndex(pDevice, x, y, Pixel);
        }
      }
    }
    break;
  }
}

/*********************************************************************
*
*       Draw Bitmap 16 BPP, not optimized
*
* Purpose:
*   Drawing of 16bpp high color bitmaps.
*   Only required for 16bpp color depth of target. Should be removed otherwise.
*/
static void _DrawBitLine16BPP(GUI_DEVICE * pDevice, int x, int y, U16 const GUI_UNI_PTR * p, int xsize) {
	
	LCD_PIXELINDEX pixel;
	LCD_SetCursor(x,y);
	TFTLCD->REG  = lcddev.wramcmd;  //写入颜色值
	for (;xsize > 0; xsize--, x++, p++) 
	{
		pixel = *p;
        TFTLCD->RAM =pixel;
    }
}

/*********************************************************************
*
*       Draw Bitmap 32 BPP, not optimized
*
* Purpose:
*   Drawing of 32bpp true color bitmaps.
*   Only required for 32bpp color depth of target. Should be removed otherwise.
*/
static void _DrawBitLine32BPP(GUI_DEVICE * pDevice, int x, int y, uint32_t const GUI_UNI_PTR * p, int xsize) {
  for (;xsize > 0; xsize--, x++, p++) {
    _SetPixelIndex(pDevice, x, y, *p);
  }
}

/*********************************************************************
*
*       _DrawBitmap
*/
static void _DrawBitmap(GUI_DEVICE * pDevice, int x0, int y0,
                       int xSize, int ySize,
                       int BitsPerPixel, 
                       int BytesPerLine,
                       const U8 GUI_UNI_PTR * pData, int Diff,
                       const LCD_PIXELINDEX * pTrans) {
  int i;

  switch (BitsPerPixel) {
  case 1:
    for (i = 0; i < ySize; i++) {
      _DrawBitLine1BPP(pDevice, x0, i + y0, pData, Diff, xSize, pTrans);
      pData += BytesPerLine;
    }
    break;
  case 2:
    for (i = 0; i < ySize; i++) {
      _DrawBitLine2BPP(pDevice, x0, i + y0, pData, Diff, xSize, pTrans);
      pData += BytesPerLine;
    }
    break;
  case 4:
    for (i = 0; i < ySize; i++) {
      _DrawBitLine4BPP(pDevice, x0, i + y0, pData, Diff, xSize, pTrans);
      pData += BytesPerLine;
    }
    break;
  case 8:
    for (i = 0; i < ySize; i++) {
      _DrawBitLine8BPP(pDevice, x0, i + y0, pData, xSize, pTrans);
      pData += BytesPerLine;
    }
    break;
  //
  // Only required for 16bpp color depth of target. Should be removed otherwise.
  //
  case 16:
    for (i = 0; i < ySize; i++) {
      _DrawBitLine16BPP(pDevice, x0, i + y0, (const U16 *)pData, xSize);
      pData += BytesPerLine;
    }
    break;
  //
  // Only required for 32bpp color depth of target. Should be removed otherwise.
  //
  case 32:
    for (i = 0; i < ySize; i++) {
      _DrawBitLine32BPP(pDevice, x0, i + y0, (const uint32_t *)pData, xSize);
      pData += BytesPerLine;
    }
    break;
  }
}

/*********************************************************************
*
*       _InitOnce
*
* Purpose:
*   Allocates a fixed block for the context of the driver
*
* Return value:
*   0 on success, 1 on error
*/
static int _InitOnce(GUI_DEVICE * pDevice) {
  DRIVER_CONTEXT_TEMPLATE * pContext;

  if (pDevice->u.pContext == NULL) {
    pDevice->u.pContext = GUI_ALLOC_GetFixedBlock(sizeof(DRIVER_CONTEXT_TEMPLATE));
    pContext = (DRIVER_CONTEXT_TEMPLATE *)pDevice->u.pContext;
    pContext->BitsPerPixel = LCD__GetBPP(pDevice->pColorConvAPI->pfGetIndexMask());
  }
  return pDevice->u.pContext ? 0 : 1;
}

/*********************************************************************
*
*       _GetDevProp
*/
static I32 _GetDevProp(GUI_DEVICE * pDevice, int Index) {
  DRIVER_CONTEXT_TEMPLATE * pContext;

  pContext = (DRIVER_CONTEXT_TEMPLATE *)pDevice->u.pContext;
  switch (Index) {
  case LCD_DEVCAP_XSIZE:
    return pContext->xSize;
  case LCD_DEVCAP_YSIZE:
    return pContext->ySize;
  case LCD_DEVCAP_VXSIZE:
    return pContext->vxSize;
  case LCD_DEVCAP_VYSIZE:
    return pContext->vySize;
  case LCD_DEVCAP_BITSPERPIXEL:
    return pContext->BitsPerPixel;
  case LCD_DEVCAP_NUMCOLORS:
    return 0;
  case LCD_DEVCAP_XMAG:
    return 1;
  case LCD_DEVCAP_YMAG:
    return 1;
  case LCD_DEVCAP_MIRROR_X:
    return 0;
  case LCD_DEVCAP_MIRROR_Y:
    return 0;
  case LCD_DEVCAP_SWAP_XY:
    return 0;
  }
  return -1;
}

/*********************************************************************
*
*       _GetDevData
*/
static void * _GetDevData(GUI_DEVICE * pDevice, int Index) {
  GUI_USE_PARA(pDevice);
  #if GUI_SUPPORT_MEMDEV
    switch (Index) {
    case LCD_DEVDATA_MEMDEV:
      return (void *)&GUI_MEMDEV_DEVICE_16; // TBD: Has to be adapted to the right memory device depending on the used color depth!
    }
  #else
    GUI_USE_PARA(Index);
  #endif
  return NULL;
}

/*********************************************************************
*
*       _GetRect
*/
static void _GetRect(GUI_DEVICE * pDevice, LCD_RECT * pRect) {
  DRIVER_CONTEXT_TEMPLATE * pContext;

  pContext = (DRIVER_CONTEXT_TEMPLATE *)pDevice->u.pContext;
  pRect->x0 = 0;
  pRect->y0 = 0;
  pRect->x1 = pContext->vxSize - 1;
  pRect->y1 = pContext->vySize - 1;
}

/*********************************************************************
*
*       _SetOrg
*/
static void _SetOrg(GUI_DEVICE * pDevice, int x, int y) {
  LCD_X_SETORG_INFO Data = {0};

  Data.xPos = x;
  Data.yPos = y;
  LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_SETORG, (void *)&Data);
}

/*********************************************************************
*
*       Static code: Functions available by _GetDevFunc()
*
**********************************************************************
*/
/*********************************************************************
*
*       _SetVRAMAddr
*/
static void _SetVRAMAddr(GUI_DEVICE * pDevice, void * pVRAM) {
  DRIVER_CONTEXT_TEMPLATE * pContext;
  LCD_X_SETVRAMADDR_INFO Data = {0};

  _InitOnce(pDevice);
  if (pDevice->u.pContext) {
    pContext = (DRIVER_CONTEXT_TEMPLATE *)pDevice->u.pContext;
    pContext->VRAMAddr = (uint32_t)pVRAM;
    Data.pVRAM = pVRAM;
    LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_SETVRAMADDR, (void *)&Data);
  }
}

/*********************************************************************
*
*       _SetVSize
*/
static void _SetVSize(GUI_DEVICE * pDevice, int xSize, int ySize) {
  DRIVER_CONTEXT_TEMPLATE * pContext;

  _InitOnce(pDevice);
  if (pDevice->u.pContext) {
    pContext = (DRIVER_CONTEXT_TEMPLATE *)pDevice->u.pContext;
    pContext->vxSize = xSize;
    pContext->vySize = ySize;
    pContext->vxSizePhys = xSize;
  }
}

/*********************************************************************
*
*       _SetSize
*/
static void _SetSize(GUI_DEVICE * pDevice, int xSize, int ySize) {
  DRIVER_CONTEXT_TEMPLATE * pContext;
  LCD_X_SETSIZE_INFO Data = {0};

  _InitOnce(pDevice);
  if (pDevice->u.pContext) {
    pContext = (DRIVER_CONTEXT_TEMPLATE *)pDevice->u.pContext;
    pContext->vxSizePhys = (pContext->vxSizePhys == 0) ? xSize : pContext->vxSizePhys;
    pContext->xSize = xSize;
    pContext->ySize = ySize;
    Data.xSize = xSize;
    Data.ySize = ySize;
    LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_SETSIZE, (void *)&Data);
  }
}
/*********************************************************************
*
*       _Init
*/
static int  _Init(GUI_DEVICE * pDevice) {
  int r;

  r = _InitOnce(pDevice);
  r |= LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_INITCONTROLLER, NULL);
  return r;
}

/*********************************************************************
*
*       _On
*/
static void _On (GUI_DEVICE * pDevice) {
  LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_ON, NULL);
}

/*********************************************************************
*
*       _Off
*/
static void _Off (GUI_DEVICE * pDevice) {
  LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_OFF, NULL);
}

/*********************************************************************
*
*       _SetLUTEntry
*/
static void _SetLUTEntry(GUI_DEVICE * pDevice, U8 Pos, LCD_COLOR Color) {
  LCD_X_SETLUTENTRY_INFO Data = {0};

  Data.Pos   = Pos;
  Data.Color = Color;
  LCD_X_DisplayDriver(pDevice->LayerIndex, LCD_X_SETLUTENTRY, (void *)&Data);
}

/*********************************************************************
*
*       _GetDevFunc
*/
static void (* _GetDevFunc(GUI_DEVICE ** ppDevice, int Index))(void) {
  GUI_USE_PARA(ppDevice);
  switch (Index) {
  case LCD_DEVFUNC_SET_VRAM_ADDR:
    return (void (*)(void))_SetVRAMAddr;
  case LCD_DEVFUNC_SET_VSIZE:
    return (void (*)(void))_SetVSize;
  case LCD_DEVFUNC_SET_SIZE:
    return (void (*)(void))_SetSize;
  case LCD_DEVFUNC_INIT:
    return (void (*)(void))_Init;
  case LCD_DEVFUNC_ON:
    return (void (*)(void))_On;
  case LCD_DEVFUNC_OFF:
    return (void (*)(void))_Off;
  case LCD_DEVFUNC_SETLUTENTRY:
    return (void (*)(void))_SetLUTEntry;
  }
  return NULL;
}

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_DEVICE_API structure
*/
const GUI_DEVICE_API GUIDRV_Template_API = {
  //
  // Data
  //
  DEVICE_CLASS_DRIVER,
  //
  // Drawing functions
  //
  _DrawBitmap,
  _DrawHLine,
  _DrawVLine,
  _FillRect,
  _GetPixelIndex,
  _SetPixelIndex,
  _XorPixel,
  //
  // Set origin
  //
  _SetOrg,
  //
  // Request information
  //
  _GetDevFunc,
  _GetDevProp,
  _GetDevData,
  _GetRect,
};

#endif
/*************************** End of file ****************************/
