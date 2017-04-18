# STM32GENERIC

A generic [Arduino](https://www.arduino.cc/) implementation for [STM32 boards](boards.md).

## Installation - Users

TODO create boards manager package

## Installation - Developers

1. Download the latest version from [https://github.com/danieleff/STM32GENERIC]()
2. Unzip it into [Arduino]/hardware folder
3. Install Arduino Zero from the board manager for the arm-none-eabi-gcc compiler


## Uploading

Please go to the [Uploading](upload.md) page.

## List of boards

Please go to the [Boards](boards) page.

## API documentation

Please go to the [Arduino API implementation](arduino_api.md) page.

---

## Goals

* Generic implementation using `STM32 HAL API` to compile onto every STM32 board.
* For the most used boards, optimize code paths using direct register access.
* Make adding boards easy
* Make as compatible with default libraries as  possible

## General development direction

* Self-test code to run on a board to check if the API works for that variant.
* Automatic unit test compilation. This ensures that every code will work on every board.
* Automatic library compilation, to be as arduino-compatible as possible.
* Benchmarks to pinpoint bottlenecks.
* Hacker-friendly: Enable to use HAL/CMSIS; override every IRQ handler.
* Add DFU/MSC/HID/MIDI to USB. Rewrite USB to PluggableUSB to be more arduino-compatible.
* Create SDIO library
* Create I2S library
* Create CAN library
