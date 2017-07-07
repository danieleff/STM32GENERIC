# STM32GENERIC
Generic implementation of Arduino for STM32 boards using STM32 HAL. This is an alternative to the [Official implementation](https://github.com/stm32duino/Arduino_Core_STM32) 

Documentation: https://danieleff.github.io/STM32GENERIC/

## Installation - Users

TODO create boards manager package

## Installation - Developers

1. Download the latest version from [https://github.com/danieleff/STM32GENERIC](https://github.com/danieleff/STM32GENERIC)
2. Unzip it into [Arduino]/hardware folder
3. Install Arduino Zero from the board manager for the arm-none-eabi-gcc compiler

Additional instructions for F7 boards:

1. Download the [latest GNU ARM Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
2. Change compiler.path in [platform.txt](https://github.com/danieleff/STM32GENERIC/blob/master/STM32/platform.txt#L21) to point to that you downloaded.
