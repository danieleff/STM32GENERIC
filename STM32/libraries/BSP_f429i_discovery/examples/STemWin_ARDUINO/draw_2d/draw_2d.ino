/*
draw_2d.ino: using STenWin lib demo & sample, whith ARDUINO style.
  hardware:
     board:  stm32f407vet6(BlackF407VE/YL136)  http://wiki.stm32duino.com/index.php?title=STM32F407
       tft:  ili9341 240x320
  software:
     STenWin: v5.32
  huawei huaweiwx@sina.com 2017.7.2
*/

#include "BSP_F407VET6_ILI9341.h"  //YL136 STemWin config & driver lib
#include "MW_STemWin.h"            //MiddleWare /STemWin 5.32 lib
#include "STemWin.h"               //STemWin arduino style GUI
#include "display2d.h"

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
  digitalToggle(LED1);    // turn the LED off by making the voltage LOW
  display_2d();
  delay(1000);              // wait for a second

  digitalToggle(LED);    // turn the LED off by making the voltage LOW
  digitalToggle(LED1);    // turn the LED off by making the voltage LOW
  alpha_display();
  delay(1000);              // wait for a second

  digitalToggle(LED);    // turn the LED off by making the voltage LOW
  digitalToggle(LED1);    // turn the LED off by making the voltage LOW
  drawarcscale();
  delay(1000);              // wait for a second

  digitalToggle(LED);    // turn the LED off by making the voltage LOW
  digitalToggle(LED1);    // turn the LED off by making the voltage LOW
  draw_graph();
  delay(1000);              // wait for a second

  digitalToggle(LED);    // turn the LED off by making the voltage LOW
  digitalToggle(LED1);    // turn the LED off by making the voltage LOW
  draw_polygon();
  delay(1000);              // wait for a second
}
