#MapleMini F103CB

Below are the pins usable for the peripherals. Pins in **bold** are the default.

## SPI

Instance |MOSI|MISO|SCK|
-|-|-|-|
SPI1|**4 (PA7)**, 17 (PB5)|**5 (PA6)**, 18 (PB4)|**6 (PA5)**, 19 (PB3)|
SPI2|**28 (PB15)**|**29 (PB14)**|**30 (PB13)**|

## I2C

Instance |SDA|SCL|
-|-|-|
I2C1|**15 (PB7)**, 34 (PB9)|**16 (PB6)**, 32 (PB8)|
I2C2|**0 (PB11)**|**1 (PB10)**|

## USART

Instance |RX|TX|
-|-|-|
USART1|**25 (PA10)**, 15 (PB7)|**26 (PA9)**, 16 (PB6)|
USART2|**8 (PA3)**|**9 (PA2)**|
USART3|**0 (PB11)**|**1 (PB10)**|

## TIM

Instance |CH1|CH2|CH3|CH4|
-|-|-|-|-|
TIM1|**27 (PA8)**, <span style="text-decoration: overline">4 (PA7)</span>, <span style="text-decoration: overline">30 (PB13)</span>|**26 (PA9)**, <span style="text-decoration: overline">3 (PB0)</span>, <span style="text-decoration: overline">29 (PB14)</span>|**25 (PA10)**, <span style="text-decoration: overline">33 (PB1)</span>, <span style="text-decoration: overline">28 (PB15)</span>|**24 (PA11)**|
TIM2|**11 (PA0)**, 20 (PA15)|**10 (PA1)**, 19 (PB3)|**9 (PA2)**, 1 (PB10)|**8 (PA3)**, 0 (PB11)|
TIM3|**5 (PA6)**, 18 (PB4)|**4 (PA7)**, 17 (PB5)|**3 (PB0)**|**33 (PB1)**|
TIM4|**16 (PB6)**|**15 (PB7)**|**32 (PB8)**|**34 (PB9)**|

## ADC 

Instance | Channel | Pin
-|-|-
ADC1|IN0|11 (PA0)|
ADC1|IN1|10 (PA1)|
ADC1|IN2|9 (PA2)|
ADC1|IN3|8 (PA3)|
ADC1|IN4|7 (PA4)|
ADC1|IN5|6 (PA5)|
ADC1|IN6|5 (PA6)|
ADC1|IN7|4 (PA7)|
ADC1|IN8|3 (PB0)|
ADC1|IN9|33 (PB1)|
ADC2|IN0|11 (PA0)|
ADC2|IN1|10 (PA1)|
ADC2|IN2|9 (PA2)|
ADC2|IN3|8 (PA3)|
ADC2|IN4|7 (PA4)|
ADC2|IN5|6 (PA5)|
ADC2|IN6|5 (PA6)|
ADC2|IN7|4 (PA7)|
ADC2|IN8|3 (PB0)|
ADC2|IN9|33 (PB1)|

## GPIO 

Pin | Peripheral signal available on the pin | Board macro
-|-|-
0 (PB11) |ADC1_EXTI11, ADC2_EXTI11, I2C2_SDA, TIM2_CH4, USART3_RX||
1 (PB10) |I2C2_SCL, TIM2_CH3, USART3_TX||
2 (PB2) |||
3 (PB0) |ADC1_IN8, ADC2_IN8, TIM1_CH2N, TIM3_CH3||
4 (PA7) |ADC1_IN7, ADC2_IN7, SPI1_MOSI, TIM1_CH1N, TIM3_CH2|**MOSI**|
5 (PA6) |ADC1_IN6, ADC2_IN6, SPI1_MISO, TIM1_BKIN, TIM3_CH1|**MISO**|
6 (PA5) |ADC1_IN5, ADC2_IN5, SPI1_SCK|**SCK**|
7 (PA4) |ADC1_IN4, ADC2_IN4, SPI1_NSS, USART2_CK|**SS**|
8 (PA3) |ADC1_IN3, ADC2_IN3, TIM2_CH4, USART2_RX||
9 (PA2) |ADC1_IN2, ADC2_IN2, TIM2_CH3, USART2_TX||
10 (PA1) |ADC1_IN1, ADC2_IN1, TIM2_CH2, USART2_RTS||
11 (PA0) |ADC1_IN0, ADC2_IN0, SYS_WKUP, TIM2_CH1, TIM2_ETR, USART2_CTS||
12 (PC15) |ADC1_EXTI15, ADC2_EXTI15, RCC_OSC32_OUT||
13 (PC14) |RCC_OSC32_IN||
14 (PC13) |RTC_OUT, RTC_TAMPER||
15 (PB7) |I2C1_SDA, TIM4_CH2, USART1_RX|**SDA**|
16 (PB6) |I2C1_SCL, TIM4_CH1, USART1_TX|**SCL**|
17 (PB5) |I2C1_SMBA, SPI1_MOSI, TIM3_CH2||
18 (PB4) |SPI1_MISO, SYS_NJTRST, TIM3_CH1||
19 (PB3) |SPI1_SCK, SYS_JTDO-TRACESWO, TIM2_CH2||
20 (PA15) |SPI1_NSS, SYS_JTDI, TIM2_CH1, TIM2_ETR||
21 (PA14) |SYS_JTCK-SWCLK||
22 (PA13) |SYS_JTMS-SWDIO||
23 (PA12) |CAN_TX, TIM1_ETR, USART1_RTS, USB_DP||
24 (PA11) |CAN_RX, TIM1_CH4, USART1_CTS, USB_DM||
25 (PA10) |TIM1_CH3, USART1_RX||
26 (PA9) |TIM1_CH2, USART1_TX||
27 (PA8) |RCC_MCO, TIM1_CH1, USART1_CK||
28 (PB15) |SPI2_MOSI, TIM1_CH3N||
29 (PB14) |SPI2_MISO, TIM1_CH2N, USART3_RTS||
30 (PB13) |SPI2_SCK, TIM1_CH1N, USART3_CTS||
31 (PB12) |I2C2_SMBA, SPI2_NSS, TIM1_BKIN, USART3_CK||
32 (PB8) |CAN_RX, I2C1_SCL, TIM4_CH3||
33 (PB1) |ADC1_IN9, ADC2_IN9, TIM1_CH3N, TIM3_CH4|**LED_BUILTIN**|
34 (PB9) |CAN_TX, I2C1_SDA, TIM4_CH4||
35 (PD0) |RCC_OSC_IN||
36 (PD1) |RCC_OSC_OUT||
