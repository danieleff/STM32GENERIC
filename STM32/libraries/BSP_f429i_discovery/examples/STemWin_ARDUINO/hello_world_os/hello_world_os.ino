
/*
  hello_world.ino: using STenWin lib demo & sample, whith ARDUINO style.
  hardware:
     board:  STM32F429ZI discovery
       tft:  ili9341 240x320
  software:
     STenWin: v5.32
  huawei huaweiwx@sina.com 2017.7.2
*/
#include <BSP_f429i_discovery.h>      //F429discovery STemWin config & driver lib
#include <FreeRTOS.h>
#include <MW_STemWin.h>
#include <STemWin.h>                  //STemWin 5.32 lib

#define LED  LED_BUILTIN
#define LED1 LED_BUILTIN1

STemWinGUI GUI;

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

void MainTask (void __attribute__ ((unused)) *argument)
{
 GUI.Init();
 /* Activate the use of memory device feature */
  WM_SetCreateFlags(WM_CF_MEMDEV);
  /* USER CODE BEGIN Task2 */
  /* Infinite loop */
  for (;;)
  {
    STemWin_TextDemo();
    vTaskDelay(1000);              // wait for a second
  }
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
  xTaskCreate(MainTask,
              NULL,
              configMINIMAL_STACK_SIZE,
              NULL,
              4,
              NULL);
  vTaskStartScheduler();
}

// the loop function runs over and over again forever
void loop() {
  /*
    !!! this no runed in freertos  !!!
  */
}


void STemWin_TextDemo(void)
{
  int i;
  char acText[] = "This example demostrates text wrapping";
  GUI_RECT Rect = {50, 24, 186, 100};
  GUI_WRAPMODE aWm[] = {GUI_WRAPMODE_NONE,
                        GUI_WRAPMODE_CHAR,
                        GUI_WRAPMODE_WORD
                       };

  GUI.SetBkColor(GUI_BLUE);   //设置背景颜色
  GUI.Clear();            //清屏
  vTaskDelay(1000);              // wait for a second

  GUI.SetFont(&GUI_Font24_ASCII); //设置字体
  GUI.SetColor(GUI_YELLOW);       //设置字体颜色
  GUI.DispString("HELLO WORD!");
 vTaskDelay(1000);              // wait for a second

  GUI.SetFont(&GUI_Font8x16);   //设置字体
  GUI.SetPenSize(10);       //设置笔大小
  GUI.SetColor(GUI_RED);      //红色字体
  GUI.DrawLine(10, 50, 200, 130); //绘线
  vTaskDelay(1000);              // wait for a second

  GUI.DrawLine(10, 130, 200, 50); //绘线
  vTaskDelay(1000);              // wait for a second

  GUI.SetBkColor(GUI_BLACK);    //设置黑色背景
  GUI.SetColor(GUI_WHITE);    //设置字体颜色为白色
  GUI.SetTextMode(GUI_TM_NORMAL); //正常模式
  GUI.DispStringHCenterAt("GUI_TM_NORMAL", 100, 100);
  vTaskDelay(1000);              // wait for a second

  GUI.SetTextMode(GUI_TM_REV);  //反转文本
  GUI.DispStringHCenterAt("GUI_TM_REV"   , 100, 116);
  vTaskDelay(1000);              // wait for a second

  GUI.SetTextMode(GUI_TM_TRANS);  //透明文本
  GUI.DispStringHCenterAt("GUI_TM_TRANS" , 100, 132);
  vTaskDelay(1000);              // wait for a second

  GUI.SetTextMode(GUI_TM_XOR);    //异或文本
  GUI.DispStringHCenterAt("GUI_TM_XOR"   , 100, 148);
  vTaskDelay(1000);              // wait for a second

  GUI.SetTextMode(GUI_TM_TRANS | GUI_TM_REV); //透明反转文本
  GUI.DispStringHCenterAt("GUI_EM_TRANS|GUI_TM_REV", 100, 164);
  vTaskDelay(1000);              // wait for a second

  GUI.SetTextMode(GUI_TM_TRANS);  //透明文本
  for (i = 0; i < 3; i++)
  {
    GUI.SetColor(GUI_WHITE);
    GUI.FillRectEx(&Rect);
    GUI.SetColor(GUI_BLACK);
    GUI.DispStringInRectWrap(acText, &Rect, GUI_TA_LEFT, aWm[i]); //在当前窗口指定的矩形区域内显示字符串(并可自动换行)
    Rect.y0 += 105;
    Rect.y1 += 105;
    vTaskDelay(1000);              // wait for a second
  }
}

