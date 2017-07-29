#ifndef FLASHBLOCKDEVICE_H
#define FLASHBLOCKDEVICE_H

#include "USBDevice.h"

#include "BlockDevice.h"

#include "FlashVariables.h"

typedef void (*listener)(uint32_t block, size_t blocks);

class FlashBlockDevice: public BlockDevice {
public:
    FlashBlockDevice(uint32_t blockCount, FlashVariables<512> *flashVariables): blockCount(blockCount), flashVariables(flashVariables) {};

    bool begin();
    bool end();

    uint32_t getBlockCount() {
        return blockCount;
    }

    bool readBlocks(uint32_t block, uint8_t* dst, size_t blocks);
    bool writeBlocks(uint32_t block, uint8_t* src, size_t blocks);

    void setReadListener(listener l) {readListener = l;};
    void setWriteListener(listener l) {writeListener = l;};

private:
    uint32_t blockCount;
    FlashVariables<512> *flashVariables;
    listener readListener = NULL;
    listener writeListener = NULL;

};

#endif /* MASSSTORAGE_H_ */
