/*
  hello_world.ino: using STenWin lib demo & sample, whith ARDUINO style.
  hardware:
     board:  stm32f407vet6(BlackF407VE/YL136)  http://wiki.stm32duino.com/index.php?title=STM32F407
       tft:  ili9341 240x320
  software:
     STenWin: v5.32
  huawei huaweiwx@sina.com 2017.7.2
*/

#include "BSP_BLACK_F407VE.h"      //  BLACK_F407VE STemWin config & driver lib
#include "MW_STemWin.h"            //MiddleWare /STemWin 5.32 lib
#include "STemWin.h"               //STemWin arduino style GUI

#define LED1  LED_BUILTIN
#define LED2 LED_BUILTIN1

STemWinGUI GUI;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalToggle(LED1);    // turn the LED off by making the voltage LOW
  GUI.Init();            //init LCD & STwinGUI
}

//emwin text demo

void led_blink(void) {
  digitalToggle(LED1);    // turn the LED off by making the voltage LOW
  digitalToggle(LED2);    // turn the LED off by making the voltage LOW
}

void emWin_TextDemo(void)
{
  int i;
  char acText[] = "This example demostrates text wrapping";
  GUI_RECT Rect = {24, 24, 160, 100};
  GUI_WRAPMODE aWm[] = {GUI_WRAPMODE_NONE,
                          GUI_WRAPMODE_CHAR,
                          GUI_WRAPMODE_WORD
                       };
  for (i = 0; i < 3; i++)
  {
    GUI.SetBkColor(GUI_BLUE);   //设置背景颜色
    GUI.Clear();          	//清屏
    led_blink();
    delay(1000);              // wait for a second

    GUI.SetFont(&GUI_Font24_ASCII); //设置字体
    GUI.SetColor(GUI_YELLOW);       //设置字体颜色
    GUI.DispString("HELLO WORD!");
    led_blink();
    delay(1000);              // wait for a second

    GUI.SetFont(&GUI_Font8x16);   //设置字体
    GUI.SetPenSize(10);       //设置笔大小
    GUI.SetColor(GUI_RED);      //红色字体
    GUI.DrawLine(10, 50, 200, 130); //绘线
    led_blink();
    delay(1000);              // wait for a second

    GUI.DrawLine(10, 130, 200, 50); //绘线
    led_blink();
    delay(1000);              // wait for a second

    GUI.SetBkColor(GUI_BLACK);    //设置黑色背景
    GUI.SetColor(GUI_WHITE);    //设置字体颜色为白色
    GUI.SetTextMode(GUI_TM_NORMAL); //正常模式
    GUI.DispStringHCenterAt("GUI_TM_NORMAL", 100, 100);
    led_blink();
    delay(1000);              // wait for a second

    GUI.SetTextMode(GUI_TM_REV);  //反转文本
    GUI.DispStringHCenterAt("GUI_TM_REV"   , 100, 116);
    led_blink();
    delay(1000);              // wait for a second

    GUI.SetTextMode(GUI_TM_TRANS);  //透明文本
    GUI.DispStringHCenterAt("GUI_TM_TRANS" , 100, 132);
    led_blink();
    delay(1000);              // wait for a second

    GUI.SetTextMode(GUI_TM_XOR);    //异或文本
    GUI.DispStringHCenterAt("GUI_TM_XOR"   , 100, 148);
    led_blink();
    delay(1000);              // wait for a second

    GUI.SetTextMode(GUI_TM_TRANS | GUI_TM_REV); //透明反转文本
    GUI.DispStringHCenterAt("GUI_EM_TRANS|GUI_TM_REV", 100, 164);
    led_blink();
    delay(1000);              // wait for a second

    GUI.SetTextMode(GUI_TM_TRANS);  //透明文本
 //   GUI.SetColor(GUI_WHITE);
 //   GUI.FillRectEx(&Rect);
 //   GUI.SetColor(GUI_BLACK);
    GUI.DispStringInRectWrap(acText, &Rect, GUI_TA_LEFT, aWm[i]); //在当前窗口指定的矩形区域内显示字符串(并可自动换行)
    Rect.x0 += 70;
    Rect.x1 += 70;
    led_blink();
    delay(5000);              // wait for a second
  }
}

void loop() {
  emWin_TextDemo();
}
