#include "BSP_f429i_discovery.h"      //F429discovery STemWin config & driver lib
#include "MW_STemWin.h"               //STemWin 5.32 lib
#include "STemWin.h"                  //STemWin 5.32 lib
extern "C" void MainTask(void);

#define LED1  LED_BUILTIN
#define LED2 LED_BUILTIN1

STemWinGUI GUI;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalToggle(LED1);    // turn the LED off by making the voltage LOW
  GUI.Init();                       //init LCD & GUI
  /* Activate the use of memory device feature */
  WM_SetCreateFlags(WM_CF_MEMDEV);
  MainTask();
}

void loop() {
  // put your main code here, to run repeatedly:
}



