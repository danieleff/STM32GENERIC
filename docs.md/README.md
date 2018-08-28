# STM32GENERIC

A generic [Arduino](https://www.arduino.cc/) implementation for [STM32 boards](boards.md).

## Installation - Users

TODO create boards manager package

## Installation - Developers

1. Download the latest version from [https://github.com/danieleff/STM32GENERIC](https://github.com/danieleff/STM32GENERIC)
2. Unzip it into [Arduino]/hardware folder
3. Install Arduino Zero from the board manager for the arm-none-eabi-gcc compiler

Additional instructions for F7 boards:

1. Download the [latest GNU ARM Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
2. Change compiler.path in [platform.txt](https://github.com/danieleff/STM32GENERIC/blob/master/STM32/platform.txt#L21) to point to that you downloaded.

## Uploading

Please go to the [Uploading](upload.md) page.

## List of boards

Please go to the [Boards](boards) page.

## API documentation

Please go to the [Arduino API implementation](arduino_api.md) page.
