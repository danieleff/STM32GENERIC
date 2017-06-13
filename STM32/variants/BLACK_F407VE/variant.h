// board: Black407VE ( STM32F407VE )
// http://wiki.stm32duino.com/images/5/5c/STM32_F4VE_SCHEMATIC.PDF

#ifndef VARIANT_H
#define VARIANT_H

//*************** LED definitions *************************
#define LED_BUILTIN PA6
#define STM32_LED_BUILTIN_ACTIVE_LOW

#define LED0_BUILTIN PA6 // same as LED_BUILTIN
#define LED1_BUILTIN PA7

//*************** on board buttons*************************

#define USER_BUTTON0 PA0 // wakeup, active high
#define USER_BUTTON1 PE3 // active low
#define USER_BUTTON2 PE4 // active low

#define USER_BTN USER_BUTTON0 // STM naming compatibility

//*************** SPI pins ********************************
#define MOSI PA7
#define MISO PA6
#define SCK PA5
#define SS PA4

//*************** I2C pins ********************************
#define SDA PB7
#define SCL PB6

#endif
