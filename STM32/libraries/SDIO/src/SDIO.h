#ifndef _SDIO_H_INCLUDED
#define _SDIO_H_INCLUDED

#include "stdint.h"

class SDIOClass {
    HAL_SD_ErrorTypedef error;

  public:
    uint8_t begin();
    uint8_t end();
    uint8_t readBlocks(uint32_t block, uint8_t* dst, size_t blocks);
    uint8_t writeBlocks(uint32_t block, const uint8_t* src, size_t blocks);
};

#endif
