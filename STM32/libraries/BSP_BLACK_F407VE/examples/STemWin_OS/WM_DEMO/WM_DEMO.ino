#include "BSP_BLACK_F407VE.h"
#include "FreeRTOS.h"
#include "MW_STemWin.h"
#include "sample.h"

#define LED  LED_BUILTIN
#define LED1 LED_BUILTIN1

extern "C"  void GUI_X_ErrorOut(const char * s);

static void myTask1(void  __attribute__ ((unused)) *argument)
{
  /*Task setup*/
  pinMode(LED, OUTPUT);
  uint32_t i = 0;
  /* USER CODE BEGIN Task1 */
  /* Infinite loop */
  for (;;)
  {
    digitalToggle(LED);    // turn the LED off by making the voltage LOW
    vTaskDelay(950);              // wait for a second
  }
  /* USER CODE END Task1 */
}

static void myTask2(void __attribute__ ((unused)) *argument)
{
  /*Task setup*/
  pinMode(LED1, OUTPUT);
  /* USER CODE BEGIN Task2 */
  /* Infinite loop */
  for (;;)
  {
    digitalToggle(LED1);    // turn the LED off by making the voltage LOW
    vTaskDelay(200);              // wait for a second
  }
  /* USER CODE END Task2 */
}

void MainTask1 (void __attribute__ ((unused)) *argument)
{
  TFTLCD_Init();
  /*Task setup*/
  GUI_Init();
  /* Activate the use of memory device feature */
  WM_SetCreateFlags(WM_CF_MEMDEV);
  /* USER CODE BEGIN Task2 */
  /* Infinite loop */
  _ShowDemo();
  /* USER CODE END Task2 */
}

// the setup function runs once when you press reset or power the board.
void setup() {
  Serial.begin(115200);
  //  osThreadDef(task1Name, myTask1, osPriorityNormal, 0, 128);
  //  myTask1Handle=osThreadCreate(osThread(task1Name), NULL);
  xTaskCreate(myTask1,
              NULL,
              configMINIMAL_STACK_SIZE,
              NULL,
              3,
              NULL);
  xTaskCreate(myTask2,
              NULL,
              configMINIMAL_STACK_SIZE,
              NULL,
              3,
              NULL);
  xTaskCreate(MainTask1,
              NULL,
              configMINIMAL_STACK_SIZE,
              NULL,
              4,
              NULL);
  vTaskStartScheduler();
}

// the loop function runs over and over again forever
void loop() {
}

