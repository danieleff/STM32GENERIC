#ifndef _SDIO_H_INCLUDED
#define _SDIO_H_INCLUDED

#include "stdint.h"

class SDIOClass {

  public:
    uint8_t begin();
    uint8_t end();
    uint8_t readBlocks(uint32_t block, uint8_t* dst, size_t blocks);
    uint8_t writeBlocks(uint32_t block, const uint8_t* src, size_t blocks);

    //TODO ERASE NOT TESTED!
    uint8_t eraseBlocks(uint32_t startBlock, uint32_t endBlock);

    HAL_SD_ErrorTypedef getError();

    uint32_t getBlockSize();
    uint64_t getCapacity();
    uint8_t getType();
    uint16_t getRCA();
    HAL_SD_CIDTypedef getCID();
    HAL_SD_CSDTypedef getCSD();

    SD_HandleTypeDef handle;
    HAL_SD_CardInfoTypedef SDCardInfo;

    HAL_SD_ErrorTypedef error;
};

#endif
