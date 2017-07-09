//STemWin for ARDUINO 
//huaweiwx<huaweiwx@sina.com>

#ifndef __STEMWIN_ARDUINO_H__
#define __STEMWIN_ARDUINO_H__

#if __has_include("MW_STemWin.h")

#ifdef __cplusplus

/* class */
class STemWinGUI : public emWinGUI{
  public:
    STemWinGUI(){};
int Init(void){
	return GUI_Init();
    };
};

#endif //__cplusplus
#endif  //STemWin

#endif //__STEMWIN_ARDUINO_H__
