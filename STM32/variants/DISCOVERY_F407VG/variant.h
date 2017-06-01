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

#endif
