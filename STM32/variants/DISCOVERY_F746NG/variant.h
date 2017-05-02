#ifndef VARIANT_H
#define VARIANT_H

#define LED_BUILTIN PI1

#define MOSI PB5
#define MISO PB4
#define SCK PB3
#define SS PA4

#define SDA PB9
#define SCL PB8

#define I2C1_100KHZ_TIMING 0x20404768
#define I2C2_100KHZ_TIMING 0x20404768
#define I2C3_100KHZ_TIMING 0x20404768
#define I2C4_100KHZ_TIMING 0x20404768

enum {
   PC7 , //D0
   PC6 , //D1
   PG6 , //D2
   PB4 , //D3
   PG7 , //D4
   PA8 , //D5
   PH6 , //D6
   PI3 , //D7
   PI2 , //D8
   PA15, //D9
   PI0 , //D10
   PB15, //D11
   PB14, //D12
   PI1 , //D13
   PB9 , //D14
   PB8 , //D15
   PA0 , //D16 / A0
   PF10, //D17 / A1
   PF9 , //D18 / A2
   PF8 , //D19 / A3
   PF7 , //D20 / A4
   PF6 , //D21 / A5
   PA1 ,
   PA2 ,
   PA3 ,
   PA4 ,
   PA5 ,
   PA6 ,
   PA7 ,
   PA9 ,
   PA10,
   PA11,
   PA12,
   PA13,
   PA14,
   PB0 ,
   PB1 ,
   PB2 ,
   PB3 ,
   PB5 ,
   PB6 ,
   PB7 ,
   PB10,
   PB11,
   PB12,
   PB13,
   PC0 ,
   PC1 ,
   PC2 ,
   PC3 ,
   PC4 ,
   PC5 ,
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
   PF0 ,
   PF1 ,
   PF2 ,
   PF3 ,
   PF4 ,
   PF5 ,
   PF11,
   PF12,
   PF13,
   PF14,
   PF15,
   PG0 ,
   PG1 ,
   PG2 ,
   PG3 ,
   PG4 ,
   PG5 ,
   PG8 ,
   PG9 ,
   PG10,
   PG11,
   PG12,
   PG13,
   PG14,
   PG15,
   PH0 ,
   PH1 ,
   PH2 ,
   PH3 ,
   PH4 ,
   PH5 ,
   PH7 ,
   PH8 ,
   PH9 ,
   PH10,
   PH11,
   PH12,
   PH13,
   PH14,
   PH15,
   PI4 ,
   PI5 ,
   PI6 ,
   PI7 ,
   PI8 ,
   PI9 ,
   PI10,
   PI11,
   PI12,
   PI13,
   PI14,
   PI15,
   PJ0 ,
   PJ1 ,
   PJ2 ,
   PJ3 ,
   PJ4 ,
   PJ5 ,
   PJ6 ,
   PJ7 ,
   PJ8 ,
   PJ9 ,
   PJ10,
   PJ11,
   PJ12,
   PJ13,
   PJ14,
   PJ15,
   PK0 ,
   PK1 ,
   PK2 ,
   PK3 ,
   PK4 ,
   PK5 ,
   PK6 ,
   PK7 ,
NUM_PINS,
};

#endif
