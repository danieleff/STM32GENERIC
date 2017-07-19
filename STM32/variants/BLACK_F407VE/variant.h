#ifndef VARIANT_H
#define VARIANT_H

#define BOARD_NR_LED            2 
#define LED_BUILTIN             PA6
#define LED_BUILTIN1            PA7
#define LED_BUILTIN_MASK     0x01  
#define STM32_LED_BUILTIN_ACTIVE_LOW

#define BOARD_NR_KEY          3
#define KEY_BOTTON           PA0    //1
#define KEY_BOTTON1          PE3    //0
#define KEY_BOTTON2          PE4    //0
#define KEY_BOTTON_MASK     0x01


#define MOSI PA7
#define MISO PA6
#define SCK  PA5
#define SS   PA4

#define SDA PB7
#define SCL PB6


#endif
