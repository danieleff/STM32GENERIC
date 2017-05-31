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

#define VARIANT_PIN_LIST \
   PIN(A,0) ,\
   PIN(A,1) ,\
   PIN(A,2) ,\
   PIN(A,3) ,\
   PIN(A,4) ,\
   PIN(A,5) ,\
   PIN(A,6) ,\
   PIN(A,7) ,\
   PIN(A,8) ,\
   PIN(A,9) ,\
   PIN(A,10),\
   PIN(A,11),\
   PIN(A,12),\
   PIN(A,13),\
   PIN(A,14),\
   PIN(A,15),\
   PIN(B,0) ,\
   PIN(B,1) ,\
   PIN(B,2) ,\
   PIN(B,3) ,\
   PIN(B,4) ,\
   PIN(B,5) ,\
   PIN(B,6) ,\
   PIN(B,7) ,\
   PIN(B,8) ,\
   PIN(B,9) ,\
   PIN(B,10),\
   PIN(B,11),\
   PIN(B,12),\
   PIN(B,13),\
   PIN(B,14),\
   PIN(B,15),\
   PIN(C,0) ,\
   PIN(C,1) ,\
   PIN(C,2) ,\
   PIN(C,3) ,\
   PIN(C,4) ,\
   PIN(C,5) ,\
   PIN(C,6) ,\
   PIN(C,7) ,\
   PIN(C,8) ,\
   PIN(C,9) ,\
   PIN(C,10),\
   PIN(C,11),\
   PIN(C,12),\
   PIN(C,13),\
   PIN(C,14),\
   PIN(C,15),\
   PIN(D,0) ,\
   PIN(D,1) ,\
   PIN(D,2) ,\
   PIN(D,3) ,\
   PIN(D,4) ,\
   PIN(D,5) ,\
   PIN(D,6) ,\
   PIN(D,7) ,\
   PIN(D,8) ,\
   PIN(D,9) ,\
   PIN(D,10),\
   PIN(D,11),\
   PIN(D,12),\
   PIN(D,13),\
   PIN(D,14),\
   PIN(D,15),\
   PIN(E,0) ,\
   PIN(E,1) ,\
   PIN(E,2) ,\
   PIN(E,3) ,\
   PIN(E,4) ,\
   PIN(E,5) ,\
   PIN(E,6) ,\
   PIN(E,7) ,\
   PIN(E,8) ,\
   PIN(E,9) ,\
   PIN(E,10),\
   PIN(E,11),\
   PIN(E,12),\
   PIN(E,13),\
   PIN(E,14),\
   PIN(E,15),\
   PIN(H,0) ,\
   PIN(H,1) ,\

#endif
