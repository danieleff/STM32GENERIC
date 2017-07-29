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

#include "MassStorage.h"

#include "RamBlockDevice.h"

RamBlockDevice::RamBlockDevice() {
    blockCount = 0;
    buffer = NULL;
}

RamBlockDevice::RamBlockDevice(uint32_t blockCount, bool doFormat) {
    this->blockCount = blockCount;
    this->buffer = (uint8_t *)calloc(blockCount * 512, 1);

    if (doFormat) {
        formatFat12(this, blockCount);
    }
}

void RamBlockDevice::setBuffer(uint32_t blockCount, uint8_t *buffer) {
    this->blockCount = blockCount;
    this->buffer = buffer;
};

bool RamBlockDevice::begin() {
    return true;
};
bool RamBlockDevice::end() {
    return true;
};

uint32_t RamBlockDevice::getBlockCount() {
    return blockCount;
};

bool RamBlockDevice::readBlocks(uint32_t block, uint8_t* dst, size_t blocks) {
  memcpy(dst, buffer + block * 512, blocks * 512);
  if (readListener != NULL) {
      readListener(block, blocks);
  }
  return true;
};

bool RamBlockDevice::writeBlocks(uint32_t block, uint8_t* src, size_t blocks) {
  memcpy(buffer + block * 512, src, blocks * 512);
  if (writeListener != NULL) {
      writeListener(block, blocks);
  }
  return true;
};


void RamBlockDevice::setReadListener(listener l) {
    readListener = l;
}

void RamBlockDevice::setWriteListener(listener l) {
    writeListener = l;
}

