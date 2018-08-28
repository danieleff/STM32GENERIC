#Nucleo-F030R8

Below are the pins usable for the peripherals. Pins in **bold** are the default.

## SPI

Instance |MOSI|MISO|SCK|
-|-|-|-|
SPI1|**11 (PA7)**, 4 (PB5)|**12 (PA6)**, 5 (PB4)|**13 (PA5)**, 3 (PB3)|
SPI2|**33 (PB15)**|**32 (PB14)**|**31 (PB13)**|

## I2C

Instance |SDA|SCL|
-|-|-|
I2C1|29 (PB7), **14 (PB9)**|10 (PB6), **15 (PB8)**|
I2C2|**PB11**|**6 (PB10)**|

## USART

Instance |RX|TX|
-|-|-|
USART1|**2 (PA10)**, 29 (PB7)|**8 (PA9)**, 10 (PB6)|
USART2|**0 (PA3)**, 26 (PA15)|**1 (PA2)**, 25 (PA14)|

## TIM

Instance |CH1|CH2|CH3|CH4|
-|-|-|-|-|
TIM1|**7 (PA8)**, <span style="text-decoration: overline">11 (PA7)</span>, <span style="text-decoration: overline">31 (PB13)</span>|**8 (PA9)**, <span style="text-decoration: overline">19 (PB0)</span>, <span style="text-decoration: overline">32 (PB14)</span>|**2 (PA10)**, <span style="text-decoration: overline">27 (PB1)</span>, <span style="text-decoration: overline">33 (PB15)</span>|**22 (PA11)**|
TIM3|**12 (PA6)**, 5 (PB4), 38 (PC6)|**11 (PA7)**, 4 (PB5), 9 (PC7)|**19 (PB0)**, 39 (PC8)|**27 (PB1)**, 40 (PC9)|
TIM14|**18 (PA4)**, 11 (PA7), 27 (PB1)||||
TIM15|**1 (PA2)**, 32 (PB14), <span style="text-decoration: overline">33 (PB15)</span>|**0 (PA3)**, 33 (PB15)|||
TIM16|**12 (PA6)**, 15 (PB8), <span style="text-decoration: overline">10 (PB6)</span>||||
TIM17|**11 (PA7)**, 14 (PB9), <span style="text-decoration: overline">29 (PB7)</span>||||

## ADC 

Instance | Channel | Pin
-|-|-
ADC|IN0|16 (PA0)|
ADC|IN1|17 (PA1)|
ADC|IN2|1 (PA2)|
ADC|IN3|0 (PA3)|
ADC|IN4|18 (PA4)|
ADC|IN5|13 (PA5)|
ADC|IN6|12 (PA6)|
ADC|IN7|11 (PA7)|
ADC|IN8|19 (PB0)|
ADC|IN9|27 (PB1)|
ADC|IN10|21 (PC0)|
ADC|IN11|20 (PC1)|
ADC|IN12|34 (PC2)|
ADC|IN13|35 (PC3)|
ADC|IN14|36 (PC4)|
ADC|IN15|37 (PC5)|

## GPIO 

Pin | Peripheral signal available on the pin | Board macro
-|-|-
0 (PA3) |ADC_IN3, TIM15_CH2, USART2_RX||
1 (PA2) |ADC_IN2, TIM15_CH1, USART2_TX||
2 (PA10) |TIM1_CH3, TIM17_BKIN, USART1_RX||
3 (PB3) |SPI1_SCK||
4 (PB5) |I2C1_SMBA, SPI1_MOSI, TIM3_CH2, TIM16_BKIN||
5 (PB4) |SPI1_MISO, TIM3_CH1||
6 (PB10) |I2C2_SCL||
7 (PA8) |RCC_MCO, TIM1_CH1, USART1_CK||
8 (PA9) |TIM1_CH2, TIM15_BKIN, USART1_TX||
9 (PC7) |TIM3_CH2||
10 (PB6) |I2C1_SCL, TIM16_CH1N, USART1_TX||
11 (PA7) |ADC_IN7, SPI1_MOSI, TIM1_CH1N, TIM3_CH2, TIM14_CH1, TIM17_CH1|**MOSI**|
12 (PA6) |ADC_IN6, SPI1_MISO, TIM1_BKIN, TIM3_CH1, TIM16_CH1|**MISO**|
13 (PA5) |ADC_IN5, SPI1_SCK|**LED_BUILTIN**, **SCK**|
14 (PB9) |I2C1_SDA, IR_OUT, TIM17_CH1|**SDA**|
15 (PB8) |I2C1_SCL, TIM16_CH1|**SCL**|
16 (PA0) |ADC_IN0, RTC_TAMP2, SYS_WKUP1, USART2_CTS|**A0**|
17 (PA1) |ADC_IN1, USART2_DE, USART2_RTS|**A1**|
18 (PA4) |ADC_IN4, SPI1_NSS, TIM14_CH1, USART2_CK|**A2**, **SS**|
19 (PB0) |ADC_IN8, TIM1_CH2N, TIM3_CH3|**A3**|
20 (PC1) |ADC_IN11|**A4**|
21 (PC0) |ADC_IN10|**A5**|
22 (PA11) |TIM1_CH4, USART1_CTS||
23 (PA12) |TIM1_ETR, USART1_DE, USART1_RTS||
24 (PA13) |IR_OUT, SYS_SWDIO||
25 (PA14) |SYS_SWCLK, USART2_TX||
26 (PA15) |SPI1_NSS, USART2_RX||
27 (PB1) |ADC_IN9, TIM1_CH3N, TIM3_CH4, TIM14_CH1||
28 (PB2) |||
29 (PB7) |I2C1_SDA, TIM17_CH1N, USART1_RX||
30 (PB12) |SPI2_NSS, TIM1_BKIN||
31 (PB13) |SPI2_SCK, TIM1_CH1N||
32 (PB14) |SPI2_MISO, TIM1_CH2N, TIM15_CH1||
33 (PB15) |RTC_REFIN, SPI2_MOSI, TIM1_CH3N, TIM15_CH1N, TIM15_CH2||
34 (PC2) |ADC_IN12||
35 (PC3) |ADC_IN13||
36 (PC4) |ADC_IN14||
37 (PC5) |ADC_IN15||
38 (PC6) |TIM3_CH1||
39 (PC8) |TIM3_CH3||
40 (PC9) |TIM3_CH4||
41 (PC10) |||
42 (PC11) |||
43 (PC12) |||
44 (PC13) |RTC_OUT_ALARM, RTC_OUT_CALIB, RTC_TAMP1, RTC_TS, SYS_WKUP2||
45 (PC14) |RCC_OSC32_IN||
46 (PC15) |RCC_OSC32_OUT||
47 (PD2) |TIM3_ETR||
