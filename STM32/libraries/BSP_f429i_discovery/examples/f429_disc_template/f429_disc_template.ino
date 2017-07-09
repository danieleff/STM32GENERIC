/*
  NOTE: This is a low level example for the F429 discovery board, and is not to be used as-is.
  
  Dear reader if you have the time, please rewrite this from BSP to be like all arduino libraries (most notably Arduino GFX, and LTDC).
  Thank you.
*/

#include "BSP_f429i_discovery.h"
static uint8_t LCD_Feature = 0;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);
  Serial.println(BSP_GetVersion(),DEC);  
  // initialize the BSP  LED1
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4); 
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
  BSP_SDRAM_Init();
  BSP_GYRO_Init();
  
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER);
  BSP_LCD_SelectLayer(1);
  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

  /* Clear the LCD */ 
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);  
  
  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 10, (uint8_t*)"STM32F429I BSP", CENTER_MODE);
  BSP_LCD_SetFont(&Font16);
  BSP_LCD_DisplayStringAt(0, 35, (uint8_t*)"Drivers examples", CENTER_MODE);

  BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

}

// the loop routine runs over and over again forever:
void loop() {
  BSP_LED_Toggle(LED3);
  delay(500);
  BSP_LED_Toggle(LED4);
  delay(500);               // wait for a second
}
uint8_t ubKeyPressed;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
 if (GPIO_Pin == KEY_BUTTON_PIN)
 {
   ubKeyPressed = SET;
 }
}
