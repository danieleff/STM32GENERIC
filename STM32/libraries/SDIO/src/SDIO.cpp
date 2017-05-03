//TODO add 1bit/4bit/8bit mode selection in begin()
//TODO add pin set functions
//TODO add instance set function and constructor
//TODO test compile to all variants

#include "stm32_gpio_af.h"
#include "SDIO.h"

#include "Arduino.h"

uint8_t SDIOClass::readBlocks(uint32_t block, uint8_t* dst, size_t nb) {
    error = HAL_SD_ReadBlocks(&handle, (uint32_t*)dst, (uint64_t)block * 512, 512, nb);

    if (error != SD_OK) {
        return false;
    }

    return HAL_SD_GetStatus(&handle) == SD_TRANSFER_OK;

}

uint8_t SDIOClass::writeBlocks(uint32_t block, const uint8_t* src, size_t nb) {
    error = HAL_SD_WriteBlocks(&handle, (uint32_t*)src, (uint64_t)block * 512, 512, nb);

    if (error != SD_OK) {
        return false;
    }

    return HAL_SD_GetStatus(&handle) == SD_TRANSFER_OK;
}
