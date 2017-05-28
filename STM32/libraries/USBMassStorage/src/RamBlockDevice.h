#ifndef RAMDISK_H
#define RAMDISK_H

#include "USBDevice.h"

#include "BlockDevice.h"

typedef void (*listener)(uint32_t block, size_t blocks);

class RamBlockDevice: public BlockDevice {
public:
  RamBlockDevice();
  RamBlockDevice(uint32_t blockCount, bool format = false);

  void setBuffer(uint32_t blockCount, uint8_t *buffer);

  bool begin();
  bool end();
  uint32_t getBlockCount();

  bool readBlocks(uint32_t block, uint8_t* dst, size_t blocks);
  bool writeBlocks(uint32_t block, uint8_t* src, size_t blocks);

  void setReadListener(listener);
  void setWriteListener(listener);

  uint32_t blockCount;
  uint8_t *buffer;
  private:
  listener readListener = NULL;
  listener writeListener = NULL;

};

#endif /* MASSSTORAGE_H_ */
