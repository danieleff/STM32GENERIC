/* Testing erasing / writing to flash memory
*/

#include "STM32F1FlashBlock.h"

//Last page of flash
STM32F1EmbeddedFlashBlock flash(FLASH_BASE + FLASH_LENGTH - FLASH_PAGE_SIZE, FLASH_PAGE_SIZE);

uint8_t data[4];

void setup() {
    Serial.begin(115200);

    delay(1000);
    Serial.printf("Flash page size is %d bytes\n", FLASH_PAGE_SIZE);
    Serial.printf("Test base address is %X\n", flash.baseAddress);
    Serial.println("Press a key");
}

void loop() {
    while(Serial.read() == -1);

    flash.read(0, sizeof(data), data);
    Serial.printf("Data in flash: %08X\n", *((uint32_t*)data));

    flash.erase(0, 1);

    flash.read(0, sizeof(data), data);
    Serial.printf("After erasing should be 0xFFFFFFFF: %08X\n", *((uint32_t*)data));

    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    data[3] = 4;
    flash.write(0, sizeof(data), data);

    flash.read(0, sizeof(data), data);
    Serial.printf("After writing should be 0x04030201: %08X\n", *((uint32_t*)data));

}
