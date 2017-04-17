# STM32GENERIC

A generic Arduino implementation for STM32 boards.

## Installation

1. Download the latest version from [https://github.com/danieleff/STM32GENERIC]()
2. Unzip it into [Arduino]/hardware folder
3. Install Arduino Zero board for the arm-none-eabi compiler

TODO boards manager

## Uploading

Please go to the [Uploading](upload.md) page.

## Supported boards

Please go to the [Boards](boards) page.

## API documentation

Please go to the [Arduino API implementation](arduino_api.md) page.

---

# Implementation details

### Goals

1. Generic implementation using `STM32 HAL API` to compile onto every STM32 board.
2. For the most used boards, optimize code paths using register access.
3. Make boards 
4. Automatic unit test compilation. This insures that every code will work on every board.
