#ifndef __STEMWIN_ARDUINO_H__
#define __STEMWIN_ARDUINO_H__

#ifdef __cplusplus
#if __has_include("MW_STemWin.h")

/* class */
#include "MW_STemWin.h"
#include "STemWin_Config/LCD/BSP_tft_lcd.h"

class STemWinGUI : public emWinGUI
{
  public:
      STemWinGUI(){};
	  
int Init(void){
 	   TFTLCD_Init();
	   return GUI_Init();
	};
	
  private:

};

#endif  //STemWin
#endif //__cplusplus
#endif //__STEMWIN_ARDUINO_H__