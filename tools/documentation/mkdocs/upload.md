# Uploading your sketch

Select the upload method from the Tools / upload menu option.

Method                 |  Connection | Requirements
-----------------------|------------------|----
SWD / JTAG             | SWD pins   | SWD Programmer hardware, (included in Nucleo and Discovery boards)
Mass storage           | SWD pins   | ST-Link V2.1, (included in Nucleo and Discovery boards)
System bootloader      | UART1      | BOOT0 / BOOT1 pins accessible, USB to UART adapter
USB DFU bootloader     | USB        | USB connector, Bootloader already installed using other method
USB HID bootloader     | USB        | USB connector, Bootloader already installed using other method
Black Magic Probe      | USB        | Black Magic Probe


## SWD / JTAG

Prerequisites:

* SWD Programmer hardware connected to your board (included in Nucleo and Discovery boards, otherwise purchase one)

Tool used to upload:

* Windows: ST-LINK_CLI.exe (STM32 ST-LINK utility STSW-LINK004)
* Linux: texane stlink

## Mass storage

Prerequisites:

* Nucleo and Discovery board with preinstalled mass storage upload firmware

Tool used to upload:

* nucleoFlasher, simply copy the .bin file to the mounted drive

## System bootloader

Prerequisites:

* None, every STM32 board has a system bootloader always available

Tool used to upload:

* stm32flash

## USB DFU bootloader

Prerequisites:

* The board must have the USB connector available
* Bootloader must be already installed on the board. [Download prebuilt bootloaders from here](https://github.com/rogerclarkmelbourne/STM32duino-bootloader/tree/master/bootloader_only_binaries)

Tool used to upload:

* maple_loader

## USB HID bootloader

Prerequisites:

* The board must have the USB connector available
* Bootloader must be already installed on the board. Download prebuilt bootloaders here: [STM32F1](https://github.com/Serasidis/STM32_HID_Bootloader/tree/master/bootloader_F1/bootloader_only_binaries) / [STM32F4](https://github.com/Serasidis/STM32_HID_Bootloader/tree/master/bootloader_F4/bootloader_only_binaries)

Tool used to upload:

* Windows: [hid-flash](https://github.com/Serasidis/STM32_HID_Bootloader)

## Black Magic Probe

TODO
