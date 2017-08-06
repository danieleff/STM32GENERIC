#include "stm32_def.h"
#include "FlashVariables.h"

#if defined(STM32F1)

    #warning "Using last 8K of flash for EEPROM emulation"

    #include "STM32F1FlashBlock.h"

    STM32F1EmbeddedFlashBlock block0(FLASH_LENGTH - 8 * 1024, 4 * 1024);
    STM32F1EmbeddedFlashBlock block1(FLASH_LENGTH - 4 * 1024, 4 * 1024);

    FlashVariables<1> storageBackend(&block0, &block1);

#elif defined(STM32F4)

    #warning "Using flash from 256K to 512K as EEPROM emulation"

    #include "STM32F4FlashBlock.h"

    STM32F4EmbeddedFlashBlock block0(FLASH_BASE + 2 * 128 * 1024, 128 * 1024);
    STM32F4EmbeddedFlashBlock block1(FLASH_BASE + 3 * 128 * 1024, 128 * 1024);

    FlashVariables<1> storageBackend(&block0, &block1);

#else

    #error "No default EEPROM emulation in flash set"

#endif

uint8_t stm32ReadEEPROM(const int index) {
    uint8_t ret;
    storageBackend.read(index, &ret);
    return ret;
}

void stm32WriteEEPROM(const int index, uint8_t data) {
    storageBackend.write(index, &data);
}

int stm32GetEEPROMLength() {
    return storageBackend.entryPerPage;
}

void stm32FormatEEPROM() {
    storageBackend.format();
}

