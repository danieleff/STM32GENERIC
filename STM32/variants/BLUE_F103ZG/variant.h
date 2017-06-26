// board: BLUE103ZG ( STM32F103ZGT6 )
// http://wiki.stm32duino.com/index.php?title=Vcc-gnd.com_STM32F103ZET6

#ifndef VARIANT_H
#define VARIANT_H

//*************** LED definitions *************************
#define LED_BUILTIN PC13
#define STM32_LED_BUILTIN_ACTIVE_LOW

#define LED0_BUILTIN PC13 // same as LED_BUILTIN

//*************** on board buttons*************************



//*************** SPI pins ********************************
#define MOSI PB5
#define MISO PB4
#define SCK PB3
#define SS PA4

//*************** I2C pins ********************************
#define SDA PB7
#define SCL PB6

#endif
