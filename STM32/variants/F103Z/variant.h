#ifndef VARIANT_H
#define VARIANT_H
#define  HAVE_24CXX
#define  HAVE_485
#define  HAVE_BEEP
#define  HAVE_CAN
//#define  HAVE_DS18B02
#define  HAVE_EXTSRAM
//#define  HAVE_IRREMOTE
//#define  HAVE_LDR
#define  HAVE_NAND
#define  HAVE_NOR
//#define  HAVE_NRF24L01
//#define  HAVE_OLED
//#define  HAVE_PS2
#define  HAVE_SPIFLASH
#define  HAVE_SDMMC



//LED USERDEF ZKiii
#define BOARD_NR_LED            4
#define LED_BUILTIN   PF6
#define LED_BUILTIN1  PF7
#define LED_BUILTIN2  PF8
#define LED_BUILTIN3  PF9
#define STM32_LED_BUILTIN_ACTIVE_LOW


//BOTTON DEF

#define BOARD_NR_KEY             4

#define KEY0_BUILTIN             PA0
#define KEY0_BUILTIN_MASK        0    //PRESED 1

#define KEY1_BUILTIN             PC13    //!!需改成目标板子上的具体KEY 位置!!
#define KEY1_BUILTIN_MASK        0    //0x02 PRESED 1 


#define KEY2_BUILTIN             PA8    //!!需改成目标板子上的具体KEY 位置!!   
#define KEY2_BUILTIN_MASK        0    //0x04 PRESED 1

#define KEY3_BUILTIN             PD3    //!!需改成目标板子上的具体KEY 位置!!   
#define KEY3_BUILTIN_MASK        0    //0x04 PRESED 1

#define BEEP_BUILTIN             PB10



#define MOSI PA7
#define MISO PA6
#define SCK  PA5
#define SS   PA4

#define SDA PB7
#define SCL PB6



#endif
