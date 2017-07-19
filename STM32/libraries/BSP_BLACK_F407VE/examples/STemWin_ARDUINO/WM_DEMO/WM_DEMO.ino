/* WM_DEMO.ino   using STenWin lib demo & sample, whith ARDUINO style.
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
#include "sample.h"

#define LED  LED_BUILTIN
#define LED1 LED_BUILTIN1

STemWinGUI GUI;
void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LED1, OUTPUT);
  GUI.Init();
}

void loop() {
  digitalToggle(LED);    // turn the LED off by making the voltage LOW
  digitalToggle(LED1);   // turn the LED off by making the voltage LOW
  _ShowDemo();
  delay(1000);           // wait for a second
}
