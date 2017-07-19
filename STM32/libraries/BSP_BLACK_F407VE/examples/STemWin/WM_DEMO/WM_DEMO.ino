/* WM_DEMO.ino  STemWin window Manager demo
   huawei  huaweiwx@sina.com 2017.7.2
   board:  STM32F407VET6(YL136)
*/

#include "BSP_BLACK_F407VE.h"
#include "MW_STemWin.h"
#include "sample.h"

#define LED  LED_BUILTIN
#define LED1 LED_BUILTIN1


void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LED1, OUTPUT);
  TFTLCD_Init();
  GUI_Init();
}

void loop() {
  digitalToggle(LED);    // turn the LED off by making the voltage LOW
  digitalToggle(LED1);   // turn the LED off by making the voltage LOW
  _ShowDemo();
  delay(1000);           // wait for a second
}
