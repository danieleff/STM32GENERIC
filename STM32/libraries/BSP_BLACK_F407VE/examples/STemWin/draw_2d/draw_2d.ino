#include "BSP_BLACK_F407VE.h"
#include "MW_STemWin.h"
#include "display2d.h"

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
