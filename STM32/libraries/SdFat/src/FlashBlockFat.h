#ifndef FLASH_BLOCK_FAT_H
#define FLASH_BLOCK_FAT_H

#include "SdFat.h"
#include "FlashVariables.h"

class STM32InternalFlashBlockDriver : public BaseBlockDriver  {
  public:
    bool begin(FlashVariables<512> *flashVariables) {
        this->flashVariables = flashVariables;
        return true;
    }
    bool syncBlocks() {
        return true;
    }
    bool readBlock(uint32_t block, uint8_t* dst) {
        PRINT_INFO("Reading block %d", block);

        return flashVariables->read(block, dst);
    };
    bool writeBlock(uint32_t block, const uint8_t* src) {
        PRINT_INFO("Writing block %d", block);

        return flashVariables->write(block, (uint8_t *)src);
    };
    bool readBlocks(uint32_t block, uint8_t* dst, size_t blocks) {
        PRINT_INFO("Reading blocks %d %d", block, blocks);

        for(size_t offset = 0; offset < blocks; offset++) {
            if (!flashVariables->read(block + offset, dst + offset * 512)) {
                return false;
            }
        }
        return true;
    };
    bool writeBlocks(uint32_t block, const uint8_t* src, size_t blocks) {
        PRINT_INFO("Writing blocks %d %d", block, blocks);

        for(size_t offset = 0; offset < blocks; offset++) {
            if (!flashVariables->write(block + offset, (uint8_t *)src + offset * 512)) {
                return false;
            }
        }
        return true;
    };
  private:
      FlashVariables<512> *flashVariables;
};

class FlashBlockFat : public SdFileSystem<STM32InternalFlashBlockDriver> {
public:
    FlashBlockFat(FlashVariables<512> *flashVariables): flashVariables(flashVariables) {};

    bool begin() {
        return m_card.begin(flashVariables) && SdFileSystem::begin();
    }
private:
    FlashVariables<512> *flashVariables;
};

#endif
