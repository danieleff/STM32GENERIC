#ifndef VARIANT_H
#define VARIANT_H

#define LED_BUILTIN PA5
#define LED2_BUILTIN PB14
#define LED3_BUILTIN PC9

#define MOSI PA7
#define MISO PA6
#define SCK PA5
#define SS PA15

#define SDA PB9
#define SCL PB8

#define A0 PC5
#define A1 PC4
#define A2 PC3
#define A3 PC2
#define A4 PC1
#define A5 PC0

#define D0 PA1
#define D1 PA0
#define D2 PD14
#define D3 PB0
#define D4 PA3
#define D5 PB4
#define D6 PB1
#define D7 PA4
#define D8 PB2
#define D9 PA15
#define D10 PA2
#define D11 PA7
#define D12 PA6
#define D13 PA5
#define D14 PB9
#define D15 PB8

#define USER_BUTTON PC13


#define VARIANT_PIN_LIST \
   PIN(A,1 ), /* D0 */ \
   PIN(A,0 ), /* D1 */ \
   PIN(D,14), /* D2 */ \
   PIN(B,0 ), /* D3 */ \
   PIN(A,3 ), /* D4 */ \
   PIN(B,4 ), /* D5 */ \
   PIN(B,1), /* D6 */ \
   PIN(A,4 ), /* D7 */ \
   PIN(B,2 ), /* D8 */ \
   PIN(A,15 ), /* D9 */ \
   PIN(A,2 ), /* D10 */ \
   PIN(A,7 ), /* D11 */ \
   PIN(A,6 ), /* D12 */ \
   PIN(A,5 ), /* D13 */ \
   PIN(B,9 ), /* D14 */ \
   PIN(B,8 ), /* D15 */ \
   PIN(C,5 ), /* D16 / A0 */ \
   PIN(C,4 ), /* D17 / A1 */ \
   PIN(C,3 ), /* D18 / A2 */ \
   PIN(C,2 ), /* D19 / A3 */ \
   PIN(C,1 ), /* D20 / A4 */ \
   PIN(C,0 ), /* D21 / A5 */ \
   PIN(A,8), \
   PIN(A,9), \
   PIN(A,10), \
   PIN(A,11), \
   PIN(A,12), \
   PIN(A,13), \
   PIN(A,14),  \
   PIN(B,3), \
   PIN(B,5), \
   PIN(B,6), \
   PIN(B,7 ), \
   PIN(B,12), \
   PIN(B,13), \
   PIN(B,14), /* LED2_BUILTIN */ \
   PIN(B,15), \
   PIN(C,6 ), \
   PIN(C,8 ), \
   PIN(C,9 ), \
   PIN(C,10), \
   PIN(C,11), \
   PIN(C,12), \
   PIN(C,13), \
   PIN(C,14), \
   PIN(C,15), \
   PIN(D,0 ), \
   PIN(D,1 ), \
   PIN(D,2 ), \
   PIN(D,3 ), \
   PIN(D,4 ), \
   PIN(D,5 ), \
   PIN(D,6 ), \
   PIN(D,7 ), \


#endif
