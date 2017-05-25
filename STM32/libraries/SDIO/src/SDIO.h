#ifndef _SDIO_H_INCLUDED
#define _SDIO_H_INCLUDED

#include "stdint.h"
#include "BlockDriver.h"
#include "stm32_def.h"
#define sdRdTimeout 100
#define sdWrTimeout 500
#define sdErTimeout 250
#define sd_timeout 250 // timeout in ms in the new HAL API

/*
 * Auxiliary macros to derive several names from the same values
 * They derive the name of the DMA Controller+Stream/Channel and the name of the IRQ line
 * corresponding to that name.
 * Also derive the name of ISRs.
 */
	#define _SDIO_DMA(a) DMA##a
	#define SDIO_DMA(a) _SDIO_DMA(a)
	#define _SDIO_DMA_IRQn(a) DMA##a##_IRQn
	#define SDIO_DMA_IRQn(a) _SDIO_DMA_IRQn(a)
	#define _SDIO_DMA_IRQHandler(a) DMA##a##_IRQHandler
	#define SDIO_DMA_IRQHandler(a) _SDIO_DMA_IRQHandler(a)

	#define _SDIOSetDmaIRQ(a) HAL_NVIC_SetPriority(SDIO_DMA_IRQn(a), 0xF, 0); \
                            HAL_NVIC_SetPriority(SDIO_IRQn, 0xE, 0); \
							HAL_NVIC_EnableIRQ(SDIO_DMA_IRQn(a)); \
							HAL_NVIC_EnableIRQ(SDIO_IRQn);


/*
 * MCU specific values, used by the macros above.
 */

#ifdef STM32F0
#endif
#ifdef STM32F1
    #define SDIO_Stream 1_Channel3
    #define SDIO_Channel 0
	#define _DMA_Instance_Type DMA_Channel_TypeDef
	/*
	 * These settings are not possible for F1, L1, F3 series
	 * So we define them to nothing. We should move these to a single block
	 * for all the series that are compatible.
	 */
	#define	_SDIOSetDMAChannel(hdma_handler,chan)
	#define _SDIOSetDMAFIFO(hdma_handler)

#endif
#ifdef STM32F4
    #define SDIO_Stream 2_Stream6
    #define SDIO_Channel DMA_CHANNEL_4

	#define _DMA_Instance_Type DMA_Stream_TypeDef
	#define	_SDIOSetDMAChannel(hdma_handler,chan) hdma_handler.Init.Channel = chan

	#define _SDIOSetDMAFIFO(hdma_handler)	do { hdma_handler.Init.FIFOMode = DMA_FIFOMODE_ENABLE; \
								hdma_handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL; \
								hdma_handler.Init.MemBurst = DMA_MBURST_INC4; \
								hdma_handler.Init.PeriphBurst = DMA_PBURST_INC4; } while (0)



#endif

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
    void _sdioDMACallback() { HAL_DMA_IRQHandler(&hdma_sdio); }
    void _sdioCallback() {HAL_SD_IRQHandler(&hsd);}

  private:
    SD_HandleTypeDef hsd;
    DMA_HandleTypeDef hdma_sdio;
    HAL_SD_CardInfoTypeDef SDCardInfo;
    HAL_SD_CardStateTypeDef CardState;
    HAL_SD_CardStatusTypeDef CardStatus;
    bool _useDMA = false;
};

static uint32_t m_errorLine = 0;
static uint8_t m_errorCode = SD_CARD_ERROR_INIT_NOT_CALLED;
static void (*_sdio_this);

#endif
