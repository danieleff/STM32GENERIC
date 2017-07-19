#ifndef VARIANT_H
#define VARIANT_H

//On-board LED pin number           PIN 
#define LED_BUILTIN 				PG13
#define LED_GREEN                   LED_BUILTIN
#define LED_BUILTIN1 				PG14
#define LED_RED                     LED_BUILTIN1 
#define LED_BUILTIN_MASK 			0
#define LED_RED_OTG_OVERCURRENT     PC5 // be careful with this led. When using it configure the output as PULL_UP.

//On-board user button
#define USER_BTN                    PA0  


// Connected to on board LIS3DSH accelerometer, CS: PE3
#define MOSI PA7
#define MISO PA6
#define SCK PA5
#define SS PA4

// Connected to on board CS43L22 audio
#define SDA PB9
#define SCL PB6


#endif
