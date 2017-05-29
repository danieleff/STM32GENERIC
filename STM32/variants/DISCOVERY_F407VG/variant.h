#ifndef VARIANT_H
#define VARIANT_H

//On-board LED pin number           PIN  // Arduino Pin Number         
#define LED_BUILTIN                 PD12 // 18
#define LED_GREEN                   LED_BUILTIN
#define LED_BLUE                    PD15 // 58
#define LED_RED                     PD14 // 19
#define LED_ORANGE                  PD13 // 57
#define LED_RED_OTG_OVERCURRENT     PD5 // be careful with this led. When using it configure the output as PULL_UP.

//On-board user button
#define USER_BTN                    PA0  // 2


// Connected to on board LIS3DSH accelerometer, CS: PE3
#define MOSI                        PA7
#define MISO                        PA6
#define SCK                         PA5
#define SS                          PA4

// Connected to on board CS43L22 audio
#define SDA                         PB9
#define SCL                         PB6

#define VARIANT_PIN_LIST \
   PIN(A,0 ), \
   PIN(A,1 ), \
   PIN(A,2 ), \
   PIN(A,3 ), \
   PIN(A,4 ), \
   PIN(A,5 ), \
   PIN(A,6 ), \
   PIN(A,7 ), \
   PIN(A,8 ), \
   PIN(A,9 ), \
   PIN(A,10), \
   PIN(A,11), \
   PIN(A,12), \
   PIN(A,13), \
   PIN(A,14), \
   PIN(A,15), \
   PIN(B,0 ), \
   PIN(B,1 ), \
   PIN(B,2 ), \
   PIN(B,3 ), \
   PIN(B,4 ), \
   PIN(B,5 ), \
   PIN(B,6 ), \
   PIN(B,7 ), \
   PIN(B,8 ), \
   PIN(B,9 ), \
   PIN(B,10), \
   PIN(B,11), \
   PIN(B,12), \
   PIN(B,13), \
   PIN(B,14), \
   PIN(B,15), \
   PIN(C,0 ), \
   PIN(C,1 ), \
   PIN(C,2 ), \
   PIN(C,3 ), \
   PIN(C,4 ), \
   PIN(C,5 ), \
   PIN(C,6 ), \
   PIN(C,7 ), \
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
   PIN(D,8 ), \
   PIN(D,9 ), \
   PIN(D,10), \
   PIN(D,11), \
   PIN(D,12), \
   PIN(D,13), \
   PIN(D,14), \
   PIN(D,15), \
   PIN(E,0 ), \
   PIN(E,1 ), \
   PIN(E,2 ), \
   PIN(E,3 ), \
   PIN(E,4 ), \
   PIN(E,5 ), \
   PIN(E,6 ), \
   PIN(E,7 ), \
   PIN(E,8 ), \
   PIN(E,9 ), \
   PIN(E,10), \
   PIN(E,11), \
   PIN(E,12), \
   PIN(E,13), \
   PIN(E,14), \
   PIN(E,15), \
   PIN(H,0 ), \
   PIN(H,1 ),

#define PIN(a, b) P ## a ## b

enum {
    VARIANT_PIN_LIST
    NUM_PINS,
};
#undef PIN


#endif
