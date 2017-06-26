#ifndef VARIANT_H
#define VARIANT_H

//On-board LED pin number PIN
// Arduino Pin Number
#define BOARD_NR_LED 8
#define LED_BUILTIN PE8
#define LED_BUILTIN1 PE9
#define LED_BUILTIN2 PE10
#define LED_BUILTIN3 PE11
#define LED_BUILTIN4 PE12
#define LED_BUILTIN5            PE13
#define LED_BUILTIN6 PE14
#define LED_BUILTIN7 PE15
#define LED_BUILTIN_MASK 0xFF //ON 0

#define BOARD_NR_KEY 1
#define KEY0_BUILTIN PA0
#define KEY0_BUILTIN_MASK 0x01 //PRESED 1
//On-board user button
#define USER_BTN PA0 // 2

// Connected to on board LIS3DSH accelerometer, CS: PE3
#define MOSI PA7
#define MISO PA6
#define SCK PA5
#define SS PA4

// Connected to on board CS43L22 audio
#define SDA PB9
#define SCL PB6
#endif
