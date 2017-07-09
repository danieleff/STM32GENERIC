#include "BSP_f429i_discovery.h"
#include "MW_STemWin.h"
extern "C" void MainTask(void);

void setup() {
  GUI_Init();
  /* Activate the use of memory device feature */
  WM_SetCreateFlags(WM_CF_MEMDEV);
  MainTask();
}

void loop() {
  // put your main code here, to run repeatedly:
}



