#ifndef VARIANT_H
#define VARIANT_H

#define LED_BUILTIN PA5

#define MOSI PA7
#define MISO PA6
#define SCK PA5
#define SS PA4

#define SDA PB9
#define SCL PB8

#define A0 PA0
#define A1 PA1
#define A2 PA4
#define A3 PB0
#define A4 PC1
#define A5 PC0

enum {
   PA3 , // D0
   PA2 , // D1
   PA10, // D2
   PB3 , // D3
   PB5 , // D4
   PB4 , // D5
   PB10, // D6
   PA8 , // D7
   PA9 , // D8
   PC7 , // D9
   PB6 , // D10
   PA7 , // D11
   PA6 , // D12
   PA5 , // D13
   PB9 , // D14
   PB8 , // D15
   PA0 , // D16 / A0
   PA1 , // D17 / A1
   PA4 , // D18 / A2
   PB0 , // D19 / A3
   PC1 , // D20 / A4
   PC0 , // D21 / A5
   
   PA11,
   PA12,
   PA13,
   PA14,
   PA15,
   PB1 ,
   PB2 ,
   PB7 ,
   PB12,
   PB13,
   PB14,
   PB15,
   PC2 ,
   PC3 ,
   PC4 ,
   PC5 ,
   PC6 ,
   PC8 ,
   PC9 ,
   PC10,
   PC11,
   PC12,
   PC13,
   PC14,
   PC15,
   PD2 ,
   #ifdef GPIOH
   PH0 ,
   PH1 ,
   #endif
   NUM_PINS
};

#endif
