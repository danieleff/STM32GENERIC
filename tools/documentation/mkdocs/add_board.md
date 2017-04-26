# Add your board:

## 1. Create a copy of the STM32/variants/TEMPLATE folder with a name of your choice.

TODO create TEMPLATE, for now just use an existing one

## 2. Edit the ldscript.ld file

1. Change the _estack to point to the end of RAM
2. FLASH LENGTH to be the size of RAM of the microcontroller
3. RAM LENGTH to be the size of RAM of the microcontroller

TODO CCRAM? SDRAM?

## 3. Edit the variant.h file

1. Copy the `enum {`... from `system/STM32XX/stm32_chip/stm32_STM32XXXXXX.h` file. 
2. If the board has pin name - pin number assigments (for example arduino headers), rearrange the enum accordingly.
3. Change LED_BUILTIN, MOSI, MISO... macros to point to the primary LED, primary SPI, primary I2C pins.
4. Add board specific macros if the board has extra leds, buttons, SPI CS select lines etc...

## 4. Edit the variant.cpp file

1. Copy the `const stm32_port_pin_type variant_pin_list[] = {` from `system/STM32XX/stm32_chip/stm32_STM32XXXXXX.h` file.
2. Rearrange to be the same as `enum {` in `variant.h`

## 5. Edit the systemclock_config.c file

1. Run STM32CubeMX, select the chip for your board
2. If the board has external crystal, set RCC HSE to crystal. In clock configuration, set PLL Source to HSE, set HSE to the frequency of the board crystal.
3. Set some peripherals (SPI, I2C, SDIO, USB, ...), so that the clock configuration will generate extra code needed for those peripherals, and bounds check the frequencies.
4. Go to clock configuration, set everything to maximum :), let CubeMX figure out the rest.
5. Click generate. Set toolchain/IDE to SW4STM32. Clik OK
6. From the generated src/main.c, copy `void SystemClock_Config(void) {` into your `variant/.../systemclock_config.c`

## 6. Edit the boards.txt file

TODO create template. For now, copy an existing board

1. Rename the board
2. Change `build.mcu=` to your cortex-mX version
3. Change `build.series=` to STM32XX, where XX is the chip series
4. Change `build.extra_flags=-DSTM32XXXXXX` to be the exact chip version
5. Change `upload.maximum_size=` and `upload.maximum_data_size=` to the RAM/FLASH size of the chip
6. If the external crystal is not the board default in `system/STM32XX/HAL_SRC/system_stm32XXxx.c`, add `-DHSE_VALUE=8000000` to `xxx.build.extra_flags`
7. TODO explain upload / usb / serial menu options

## 7. Restart 

Restart Arduino IDE

## 8. Run the blink example on your new board

Run examples / basic / blink
