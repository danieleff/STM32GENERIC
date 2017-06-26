#ifndef _SDIO_H_INCLUDED
#define _SDIO_H_INCLUDED

#include "stdint.h"
#include "stm32_def.h"
#define sdRdTimeout 200
#define sdWrTimeout 5000
#define sdBsyTimeout 500
#define sdErTimeout 250
#define sd_timeout 250 // timeout in ms in the new HAL API
#define SDCARD_STATUS_READY_BIT (1UL << 8)


#if defined(STM32F4) || defined(STM32F7)
	#define _SDIOSetDMAFIFO(hdma_handler)	do { hdma_handler.Init.FIFOMode = DMA_FIFOMODE_ENABLE; \
								hdma_handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL; \
								hdma_handler.Init.MemBurst = DMA_MBURST_INC4; \
								hdma_handler.Init.PeriphBurst = DMA_PBURST_INC4; } while (0)
#else
    #define _SDIOSetDMAFIFO(hdma_handler)
#endif


#ifndef SDIO
#define SDIO                            SDMMC1
#define SDIO_RESPONSE_SHORT             SDMMC_RESPONSE_SHORT
#define SDIO_WAIT_NO                    SDMMC_WAIT_NO
#define SDIO_CPSM_ENABLE                SDMMC_CPSM_ENABLE
#define SDIO_RESP1                      SDMMC_RESP1
#define SDIO_CLOCK_EDGE_RISING          SDMMC_CLOCK_EDGE_RISING
#define SDIO_CLOCK_BYPASS_DISABLE       SDMMC_CLOCK_BYPASS_DISABLE
#define SDIO_CLOCK_POWER_SAVE_DISABLE   SDMMC_CLOCK_POWER_SAVE_DISABLE
#define SDIO_BUS_WIDE_1B                SDMMC_BUS_WIDE_1B
#define SDIO_HARDWARE_FLOW_CONTROL_DISABLE SDMMC_HARDWARE_FLOW_CONTROL_DISABLE
#define SDIO_BUS_WIDE_4B                SDMMC_BUS_WIDE_4B
#define SDIO_FLAG_DCRCFAIL              SDMMC_FLAG_DCRCFAIL

#define SDIO_GetResponse                SDMMC_GetResponse
#define SDIO_CmdInitTypeDef             SDMMC_CmdInitTypeDef
#define SDIO_SendCommand                SDMMC_SendCommand
#define SDIO_TypeDef                    SDMMC_TypeDef
#define SDIO_IRQHandler                 SDMMC1_IRQHandler
#endif

// Fix typo for L4
#ifndef HAL_SD_CardStateTypeDef
#define HAL_SD_CardStateTypeDef HAL_SD_CardStateTypedef
#define HAL_SD_CardStatusTypeDef HAL_SD_CardStatusTypedef
#endif

/*
 * Aux function. Doesn't exist in HAL. Allows to pre-erase blocks when the count of blocks to write is known.
 * ACMD23
 */
uint32_t SDMMC_CmdSetWrtBlkEraseCount(SDIO_TypeDef *SDIOx, uint32_t Count);

class SDIOClass {
    HAL_StatusTypeDef state;

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
    void useDMA(bool useDMA);

    SD_HandleTypeDef hsd;
  private:
    uint32_t cardStatus();
    DMA_HandleTypeDef hdma_sdio;
    HAL_SD_CardInfoTypeDef SDCardInfo;
    HAL_SD_CardStateTypeDef CardState;
    HAL_SD_CardStatusTypeDef CardStatus;
    bool _useDMA = false;
};

static uint32_t m_errorLine = 0;
static uint8_t m_errorCode = 0x64; //TODO cleanup, SdFat errors do not belong to SDIO driver (SD_CARD_ERROR_INIT_NOT_CALLED);

#endif
