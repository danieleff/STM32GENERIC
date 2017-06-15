#ifndef SDIO_STM32_INCLUDED
#define SDIO_STM32_INCLUDED

#include "SdFat.h"

#if defined(ARDUINO_ARCH_STM32) && (defined(SD_InitTypeDef))

#include "SDIO.h"

SDIOClass sdio;

bool SdioCard::begin() {
    return sdio.begin();
}
bool SdioCard::syncBlocks() {
    return true;
}
bool SdioCard::readBlock(uint32_t block, uint8_t* dst) {
    return sdio.readBlocks(block, dst, 1);
};
bool SdioCard::writeBlock(uint32_t block, const uint8_t* src) {
    return sdio.writeBlocks(block, src, 1);
};
bool SdioCard::readBlocks(uint32_t block, uint8_t* dst, size_t nb) {
    return sdio.readBlocks(block, dst, nb);
};
bool SdioCard::writeBlocks(uint32_t block, const uint8_t* src, size_t nb) {
    return sdio.writeBlocks(block, src, nb);
};
bool SdioCard::erase(uint32_t firstBlock, uint32_t lastBlock) {
    return sdio.erase(firstBlock, lastBlock);
};
bool SdioCard::readCID(void* cid){
    return sdio.readCID(cid);
};
bool SdioCard::readCSD(void* csd){
    return sdio.readCSD(csd);
};
uint32_t SdioCard::cardSize(){
    return sdio.cardSize();
};
uint8_t SdioCard::errorCode() {
      return sdio.errorCode();
}
uint32_t SdioCard::errorData() {
  return sdio.errorData();
}

#endif

#endif
