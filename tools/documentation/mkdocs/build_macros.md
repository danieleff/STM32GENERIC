When compiling, the following build macro definitions are set:
### **`ARDUINO_ARCH_STM32`**:
Meaning: 

* The code is compiling on Arduino
* The code is compiling for STM32 chip
* CMSIS is present and useable. (For example: `GPIOA->BSRR`)
* The STM32 HAL API is present and useable. (For example: `HAL_GPIO_TogglePin(port, pin)`)

This macro does not imply the avaibility of the STM32GENERIC private functions.
### Macros specific to board:
These can be used to differentiate between boards
Meaning: The code is compiled for that chip, board and form factor.

Board | Build macro
------|-------
BLACK F407VE (V2.0)            | **`ARDUINO_BLACK_F407VE`** 
BLACK F407ZE (V3.0)            | **`ARDUINO_BLACK_F407ZE`** 
BLACK F407ZG (M4 DEMO)         | **`ARDUINO_BLACK_F407ZG`** 
BLUE F103VE vcc-gnd.com        | **`ARDUINO_BLUE_F103VE`** 
BLUE F103ZE vcc-gnd.com        | **`ARDUINO_BLUE_F103ZE`** 
BLUE F103ZG vcc-gnd.com        | **`ARDUINO_BLUE_F103ZG`** 
BluePill F103CB                | **`ARDUINO_BLUEPILL_F103C8`** 
Discovery F303VC               | **`ARDUINO_DISCO_F303VC`** 
Discovery F407VG               | **`ARDUINO_DISCO_F407VG`** 
Discovery F429ZI               | **`ARDUINO_DISCO_F429ZI`** 
Discovery F746NG               | **`ARDUINO_DISCO_F746NG`** 
Discovery L053C8               | **`ARDUINO_DISCO_L053C8`** 
GENERIC(STM32F103ZE)           | **`ARDUINO_GENERIC_F103ZE`** 
RedBull(F103ZE)                | **`ARDUINO_REDBULL`** 
GENERIC(STM32F103ZF)           | **`ARDUINO_GENERIC_F103ZF`** 
GENERIC(STM32F103ZG)           | **`ARDUINO_GENERIC_F103ZG`** 
Generic F103RF                 | **`ARDUINO_GENERIC_103RF`** 
MapleMini F103CB               | **`ARDUINO_MAPLEMINI_F103CB`** 
Nucleo-F030R8                  | **`ARDUINO_NUCLEO_F030R8`** 
Nucleo-F103RB                  | **`ARDUINO_NUCLEO_F103RB`** 
Nucleo-F303RE                  | **`ARDUINO_NUCLEO_F303RE`** 
Nucleo-F401RE                  | **`ARDUINO_NUCLEO_F401RE`** 
Nucleo-F411RE                  | **`ARDUINO_NUCLEO_F411RE`** 
Nucleo-F767ZI                  | **`ARDUINO_NUCLEO_F767ZI`** 
Nucleo-L053R8                  | **`ARDUINO_NUCLEO_L053R8`** 
Nucleo-L152RE                  | **`ARDUINO_NUCLEO_L152RE`** 
Nucleo-L476RG                  | **`ARDUINO_NUCLEO_L476RG`** 
