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
#define MOSI PB5
#define MISO PB4
#define SCK PB3
#define SS PA4

//*************** I2C pins ********************************
#define SDA PB7
#define SCL PB6


enum {
   PA0 ,
   PA1 ,
   PA2 ,
   PA3 ,
   PA4 ,
   PA5 ,
   PA6 ,
   PA7 ,
   PA8 ,
   PA9 ,
   PA10,
   PA11,
   PA12,
   PA13,
   PA14,
   PA15,
   PB0 ,
   PB1 ,
   PB2 ,
   PB3 ,
   PB4 ,
   PB5 ,
   PB6 ,
   PB7 ,
   PB8 ,
   PB9 ,
   PB10,
   PB11,
   PB12,
   PB13,
   PB14,
   PB15,
   PC0 ,
   PC1 ,
   PC2 ,
   PC3 ,
   PC4 ,
   PC5 ,
   PC6 ,
   PC7 ,
   PC8 ,
   PC9 ,
   PC10,
   PC11,
   PC12,
   PC13,
   PC14,
   PC15,
   PD0 ,
   PD1 ,
   PD2 ,
   PD3 ,
   PD4 ,
   PD5 ,
   PD6 ,
   PD7 ,
   PD8 ,
   PD9 ,
   PD10,
   PD11,
   PD12,
   PD13,
   PD14,
   PD15,
   PE0 ,
   PE1 ,
   PE2 ,
   PE3 ,
   PE4 ,
   PE5 ,
   PE6 ,
   PE7 ,
   PE8 ,
   PE9 ,
   PE10,
   PE11,
   PE12,
   PE13,
   PE14,
   PE15,
   PH0 ,
   PH1 ,
NUM_PINS,
};

#endif
