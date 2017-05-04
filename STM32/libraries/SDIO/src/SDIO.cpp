//TODO add 1bit/4bit/8bit mode selection in begin()
//TODO add pin set functions
//TODO add instance set function and constructor
//TODO test compile to all variants

#include "stm32_gpio_af.h"
#include "SDIO.h"

#include "Arduino.h"

uint8_t SDIOClass::readBlocks(uint32_t block, uint8_t* dst, size_t nb) {
    error = HAL_SD_ReadBlocks(&handle, (uint32_t*)dst, (uint64_t)block * getBlockSize(), getBlockSize(), nb);

    if (error != SD_OK) {
        return false;
    }

    return HAL_SD_GetStatus(&handle) == SD_TRANSFER_OK;

}

uint8_t SDIOClass::writeBlocks(uint32_t block, const uint8_t* src, size_t nb) {
    error = HAL_SD_WriteBlocks(&handle, (uint32_t*)src, (uint64_t)block * getBlockSize(), getBlockSize(), nb);

    if (error != SD_OK) {
        return false;
    }

    return HAL_SD_GetStatus(&handle) == SD_TRANSFER_OK;
}

uint8_t SDIOClass::eraseBlocks(uint32_t startBlock, uint32_t endBlock) {
    error = HAL_SD_Erase(&handle, (uint64_t)startBlock * getBlockSize(), (uint64_t)(endBlock + 1) * getBlockSize() - 1);

    if (error != SD_OK) {
        return false;
    }

    return HAL_SD_GetStatus(&handle) == SD_TRANSFER_OK;
}

HAL_SD_ErrorTypedef SDIOClass::getError() {
    return error;
}

uint32_t SDIOClass::getBlockSize() {
    return SDCardInfo.CardBlockSize;
}

uint64_t SDIOClass::getCapacity() {
    return SDCardInfo.CardCapacity;
}

uint8_t SDIOClass::getType() {
    return SDCardInfo.CardType;
}

uint16_t SDIOClass::getRCA() {
    return SDCardInfo.RCA;
}

HAL_SD_CIDTypedef SDIOClass::getCID() {
    return SDCardInfo.SD_cid;
}

HAL_SD_CSDTypedef SDIOClass::getCSD() {
    return SDCardInfo.SD_csd;
}
