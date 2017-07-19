#ifndef __BSP_H__
#define __BSP_H__
#include "arduino.h"


/******************** LED************************/
#define LEDn   BOARD_NR_LED
typedef enum 
{
  LED1 = LED_BUILTIN,
  LED2 = LED_BUILTIN1,
} Led_TypeDef;

/******************** JOY ************************/

/******************** BOTTON ************************/

/******************  BSP_TFT_LCD ******************/
#define BSP_TFT_LCD   ILI9341

//USE NE1 A18
#define LCD_BASE_BK11       ((uint32_t)(0x60000000 | 0x00000000)) //PD1/NE1 FSMC_BANK1_1
#define LCD_BASE_BK12     ((uint32_t)(0x60000000 | 0x04000000))   //PG9/NE2 FSMC_BANK1_2
#define LCD_BASE_BK13     ((uint32_t)(0x60000000 | 0x08000000))   //PG10/NE3 FSMC_BANK1_3
#define LCD_BASE_BK14     ((uint32_t)(0x60000000 | 0x0C000000))   //PG12/NE4 FSMC_BANK1_4

#define LCD_REG	 *(volatile uint16_t *)(LCD_BASE_BK11)
#define LCD_RAM	 *(volatile uint16_t *)(LCD_BASE_BK11 + (1 << 19))  //A18
#define TFT_LCD_BASE	((uint32_t)(LCD_BASE_BK11 +(1 << 19)-2))

#define  BL_PORT  GPIOB
#define  BL_PIN   GPIO_PIN_1

#ifdef __cplusplus
extern "C"{
#endif

void STM_FSMC_LCD_Init(void);
void STM_FSMC_LCD_Set(uint8_t _as, uint8_t _ds);

#ifdef __cplusplus
} //extern "C"
#endif

#endif   //__BSP_H__