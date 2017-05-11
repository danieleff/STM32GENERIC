#ifndef _SDIO_H_INCLUDED
#define _SDIO_H_INCLUDED

#include "stdint.h"
#include "BlockDriver.h"

class SDIOClass {
    HAL_SD_ErrorTypedef error;

  public:
    uint8_t begin();
    uint8_t end();
    uint8_t readBlocks(uint32_t block, uint8_t* dst, size_t blocks);
    uint8_t writeBlocks(uint32_t block, const uint8_t* src, size_t blocks);
    /** Erase a range of blocks.
     *
     * \param[in] firstBlock The address of the first block in the range.
     * \param[in] lastBlock The address of the last block in the range.
     *
     * \note This function requests the SD card to do a flash erase for a
     * range of blocks.  The data on the card after an erase operation is
     * either 0 or 1, depends on the card vendor.  The card must support
     * single block erase.
     *
     * \return The value true is returned for success and
     * the value false is returned for failure.
     */
    bool erase(uint32_t firstBlock, uint32_t lastBlock);
    /**
     * Read a card's CID register. The CID contains card identification
     * information such as Manufacturer ID, Product name, Product serial
     * number and Manufacturing date.
     *
     * \param[out] cid pointer to area for returned data.
     *
     * \return true for success or false for failure.
     */
    bool readCID(void* cid);
    /**
     * Read a card's CSD register. The CSD contains Card-Specific Data that
     * provides information regarding access to the card's contents.
     *
     * \param[out] csd pointer to area for returned data.
     *
     * \return true for success or false for failure.
     */
    bool readCSD(void* csd);
    /**
     * Determine the size of an SD flash memory card.
     *
     * \return The number of 512 byte data blocks in the card
     *         or zero if an error occurs.
     */
    uint32_t cardSize();
    /**
     * \return code for the last error. See SdInfo.h for a list of error codes.
     */
    uint8_t errorCode();
    uint32_t errorData();
    /** \return error line for last error. Tmp function for debug. */
    uint32_t errorLine();
};

#endif
