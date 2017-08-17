/*
  Copyright (c) 2017 Daniel Fekete

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef STM32F4FLASHBLOCK_H
#define STM32F4FLASHBLOCK_H

#include "FlashBlock.h"

class STM32F1EmbeddedFlashBlock: public FlashBlock {
public:
    STM32F1EmbeddedFlashBlock(uint32_t baseAddress, uint32_t size): FlashBlock(size), baseAddress(baseAddress) {
        HAL_FLASH_Unlock();
    };

    bool erase(uint32_t offset, uint32_t size);

    bool write(uint32_t offset, uint32_t size, uint8_t *data);

    bool read(uint32_t offset, uint32_t size, uint8_t *data);

    uint32_t baseAddress;

    uint32_t sectorError;
};


bool STM32F1EmbeddedFlashBlock::erase(uint32_t offset, uint32_t size) {
    FLASH_EraseInitTypeDef init;


    offset = ((offset + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE;

#ifdef FLASH_BANK_2
    #error "Erasing with multiple banks not yet implemented"
#endif

    init.Banks = FLASH_BANK_1;
    init.NbPages = (size + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE;
    init.PageAddress = baseAddress + offset;
    init.TypeErase = FLASH_TYPEERASE_PAGES;

    PRINT_INFO("Erasing at %8x, pages: %d", init.PageAddress, init.NbPages);

    HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&init, &sectorError);

    if (status != HAL_OK) {
        PRINT_ERROR("Failed to erase: %d", status);
        return false;
    }

    return true;
}

bool STM32F1EmbeddedFlashBlock::write(uint32_t offset, uint32_t size, uint8_t *data) {
    PRINT_DEBUG("Writing to flash at %u size %u", offset, size);

    if ((size & 3) != 0) {
        PRINT_ERROR("Write error, size not multiple of 4");
        return false;
    }

    uint16_t *d = (uint16_t*)data;
    for(size_t i=0; i<size; i+=2) {

        uint32_t address = baseAddress + offset + i;
        HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address, *d);
        if (status != HAL_OK) {
            PRINT_ERROR("Failed to program at %x: %d", address, status);
            return false;
        }
        d++;
    }
    return true;
}

bool STM32F1EmbeddedFlashBlock::read(uint32_t offset, uint32_t size, uint8_t *data) {
    PRINT_TRACE("Reading from flash at %u size %u", offset, size);

    memcpy(data, (void*)(baseAddress + offset), size);

    return true;
}

#endif
