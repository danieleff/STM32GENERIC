#ifndef MASSSTORAGE_H
#define MASSSTORAGE_H

#include "BlockDevice.h"

extern BlockDevice *getMassStorage();

void formatFat12(BlockDevice *blockDevice, uint32_t blockCount);

#endif
