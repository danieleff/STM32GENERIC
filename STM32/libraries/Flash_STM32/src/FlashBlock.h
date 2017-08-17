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

#ifndef EMBEDDED_FLASH_H
#define EMBEDDED_FLASH_H

#include "stdint.h"

#include "string.h"

#include "stdlib.h"

#include "stdio.h"

#include "Arduino.h"

/**
 * Represents a part of STM32 chip embedded flash, from baseAddress, size bytes length
 */
class FlashBlock {
public:
    FlashBlock(uint32_t size): size(size) {};

    uint32_t getSize() {
        return size;
    }

    virtual bool write(uint32_t offset, uint32_t size, uint8_t *data) = 0;

    virtual bool read(uint32_t offset, uint32_t size, uint8_t *data) = 0;

    virtual bool erase(uint32_t offset, uint32_t size) = 0;

    void print(Print &print, int chunkSize) {
        for(int i = 0; i < size; i++) {

            if (i == 4 || (i + 4) % chunkSize == 0) {
                print.print("|");
            }

            uint8_t data;
            read(i, 1, &data);
            print.printf("%02x", data);

        }
        print.print("\n");
    }

private:
    int size;
};


//For testing purposes:

class RamFlashPage: public FlashBlock {
public:
    RamFlashPage(uint32_t size): FlashBlock(size) {
        buffer = (uint8_t *)malloc(size);
        if (!buffer) {
            PRINT_ERROR("Could not allocate %u bytes for RAM", size);
        }
    };

    bool write(uint32_t offset, uint32_t size, uint8_t *data) {
        PRINT_DEBUG("Writing to ram at %u size %u", offset, size);

        memcpy(buffer + offset, data, size);

        return true;
    }

    bool read(uint32_t offset, uint32_t size, uint8_t *data) {
        PRINT_TRACE("Reading ram at %u size %u", offset, size);

        memcpy(data, buffer + offset, size);

        return true;
    }

    bool erase(uint32_t offset, uint32_t size) {
        PRINT_INFO("Erasing ram at %u size %u", offset, size);

        memset(buffer, 0xFF, size);

        return true;
    }

    uint8_t *buffer;
};

#endif
