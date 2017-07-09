#if defined(ARDUINO_ARCH_HALMX)||defined(ARDUINO_ARCH_STM32)
#include <FreeRTOS.h>
#elif defined(ARDUINO_ARCH_MAPLE_STM32F1)||defined(ARDUINO_ARCH_MAPLE_STM32F3)||defined(ARDUINO_ARCH_MAPLE_STM32F4)
#include <MapleFreeRTOS821.h>
#endif

/*
  Blink2_FreeRTOS90.ino
  Turns on 2 LEDs on/off , running in FreeRTOS V9.0.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.
  modified May 2 2017  by huaweiwx@sina.com
*/

#define LED  LED_BUILTIN
#define LED_ON bitRead(LED_BUILTIN_MASK,0)

#if defined(LED_BUILTIN1)
 #define LED1 LED_BUILTIN1
 #define LED1_ON bitRead(LED_BUILTIN_MASK,1)
#endif

#ifdef ARDUINO_NUCLEO_64
  #define    mySerial  SerialUART2
#elif defined(ARDUINO_NUCLEO_144)
   SerialUART mySerial(USART3,PD9,PD8); 
#else
  #define mySerial  SerialUART1
#endif

static void myTask1(void  __attribute__ ((unused)) *argument)
{
  /*Task setup*/
  pinMode(LED, OUTPUT);

#if defined(ARDUINO_ARCH_NUCLEO_144)  //NucleoF401 default com is uart2
//  mySerial.stm32SetRX(PD9); //for NUCLEO 767ZI
//  mySerial.stm32SetTX(PD8);
#endif

  mySerial.begin(115200);

  uint32_t i = 0;
  /* USER CODE BEGIN Task1 */
  /* Infinite loop */
  for (;;)
  {
    digitalWrite(LED, LED_ON);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay(50);              // wait for a second
    digitalToggle(LED);    // turn the LED off by making the voltage LOW
    vTaskDelay(950);              // wait for a second
    i++;
    mySerial << "Count:" << _HEX(i) << " in myTask1\n";
  }
  /* USER CODE END Task1 */
}

#if defined(LED_BUILTIN1)
static void myTask2(void __attribute__ ((unused)) *argument)
{
  /*Task setup*/
  pinMode(LED1, OUTPUT);
  /* USER CODE BEGIN Task2 */
  /* Infinite loop */
  for (;;)
  {
    digitalWrite(LED1,LED1_ON);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay(50);              // wait for a second
    digitalToggle(LED1);    // turn the LED off by making the voltage LOW
    vTaskDelay(200);              // wait for a second
  }
  /* USER CODE END Task2 */
}
#endif

// the setup function runs once when you press reset or power the board.
void setup() {
  //  osThreadDef(task1Name, myTask1, osPriorityNormal, 0, 128);
  //  myTask1Handle=osThreadCreate(osThread(task1Name), NULL);
  xTaskCreate(myTask1,
              NULL,
              configMINIMAL_STACK_SIZE,
              NULL,
              3,
              NULL);
#if defined(LED_BUILTIN1)
  xTaskCreate(myTask2,
              NULL,
              configMINIMAL_STACK_SIZE,
              NULL,
              3,
              NULL);
#endif
  vTaskStartScheduler();
  // osKernelStart(); //FreeRTOS start and never return!
}

// the loop function runs over and over again forever
void loop() {
  /*
    !!! this no runed in freertos  !!!
  */
}
