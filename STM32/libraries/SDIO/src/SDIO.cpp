//TODO add 1bit/4bit/8bit mode selection in begin()
//TODO add pin set functions
//TODO add instance set function and constructor
//TODO test compile to all variants

#include "stm32_gpio_af.h"
#include "SDIO.h"

#include "Arduino.h"


//=============================================================================
// Error function and macro.
#define sdError(code) setSdErrorCode(code, __LINE__)
inline bool setSdErrorCode(uint8_t code, uint32_t line) {
  m_errorCode = code;
  m_errorLine = line;
  return false;  // setSdErrorCode
}

uint8_t SDIOClass::begin() {
    //GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_SDIO_CLK_ENABLE();

    stm32AfSDIO4BitInit(SDIO, NULL, 0, NULL, 0,
            NULL, 0, NULL, 0, NULL, 0, NULL, 0);

    hsd.Instance = SDIO;
    hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
    hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;

    hsd.Init.ClockDiv = 0;

    state = HAL_SD_Init(&hsd);
    if (state != HAL_OK) {
        return false;
    }
    HAL_SD_GetCardStatus(&hsd, &CardStatus);

        state = HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B);
        if (state != HAL_OK) {
            return false;
        }

    /*
     * TODO: We can move this section to useDMA
     * We would need another section clearing the settings if not using DMA.
     */
    _sdio_this = (void*) this;
    __HAL_RCC_DMA2_CLK_ENABLE();

    hdma_sdio.Instance = SDIO_DMA(SDIO_Stream);
    hdma_sdio.Parent = &hsd;
    _SDIOSetDMAChannel(hdma_sdio, SDIO_Channel);
    hdma_sdio.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdio.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio.Init.Mode = DMA_PFCTRL;
    hdma_sdio.Init.Priority = DMA_PRIORITY_LOW;
    _SDIOSetDMAFIFO(hdma_sdio);

    hsd.hdmatx = &hdma_sdio;
    hsd.hdmarx = &hdma_sdio;

    _SDIOSetDmaIRQ(SDIO_Stream);

    if (HAL_DMA_Init(&hdma_sdio) != HAL_OK) {
        return false;
    }

    /*
     * Up to here
     */
    _useDMA = false;

    m_errorCode = SD_CARD_ERROR_NONE;
    return true;
}

uint8_t SDIOClass::readBlocks(uint32_t block, uint8_t* dst, size_t nb) {
    if (((uint32_t)dst & 0x3U) != 0){
        while (1); //Hang here, dst was not aligned to word, this is a problem
    }
    if (!_useDMA){
        state = HAL_SD_ReadBlocks(&hsd, dst, block, nb, (uint32_t)sd_timeout);

        if (state != HAL_OK) {
            return false;
        }

        return HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER;
    }
    else {
        hdma_sdio.Init.Direction = DMA_PERIPH_TO_MEMORY;
        if (HAL_DMA_Init(&hdma_sdio) != HAL_OK) {
            return false;
        }
        state = HAL_SD_ReadBlocks_DMA(&hsd, dst, block, nb);
        if (state != HAL_OK) {
            return false;
        }
        // We need to block here, until we implement a callback feature
        uint32_t tickstart = HAL_GetTick();
        while (hsd.State == HAL_SD_STATE_BUSY){
            if((HAL_GetTick() - tickstart) >=  sd_timeout)
            {
                /* Abort transfer and send return error */
                HAL_SD_Abort(&hsd);
                return false;
            }
        }
        if (hsd.State != HAL_SD_STATE_READY ) {
            return false;
        }

        return HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER;
    }
}

uint8_t SDIOClass::writeBlocks(uint32_t block, const uint8_t* src, size_t nb) {
    if (((uint32_t)src & 0x3) !=0){
        while (1); //Hang here, src was not aligned to word, this is a problem
    }
    if (!_useDMA){
        state = HAL_SD_WriteBlocks(&hsd,(uint8_t*) src, block, (uint32_t) nb, sd_timeout);

        if (state != HAL_OK) {
            return false;
        }
        return HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER;
    }
    else {
        hdma_sdio.Init.Direction = DMA_MEMORY_TO_PERIPH;
        if (HAL_DMA_Init(&hdma_sdio) != HAL_OK) {
            return false;
        }
        state = HAL_SD_WriteBlocks_DMA(&hsd, (uint8_t*)src, block, nb);
        if (state != HAL_OK) {
            return false;
        }
        /*
         *  We need to block here, until we implement a callback feature
         *  TODO: better check if the card is still in programming mode.
         *  Also confirm the timeout to wait from the specs, currently sd_timeout=100
         *  It may need to be multiplied by a factor depending on number of blocks.
         *  Finally, perhaps rename sd_timeout to something else
         */
        uint32_t tickstart = HAL_GetTick();
        while (hsd.State == HAL_SD_STATE_BUSY){
            if((HAL_GetTick() - tickstart) >=  sd_timeout)
            {
                /* Abort transfer and send return error */
                HAL_SD_Abort(&hsd);
                return false;
            }
        }
        if (hsd.State != HAL_SD_STATE_READY) {
            return false;
        }
        while (HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_PROGRAMMING);
        return HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER;
    }
}

bool SDIOClass::erase(uint32_t firstBlock, uint32_t lastBlock) {
    state = HAL_SD_Erase(&hsd, (uint64_t)firstBlock, (uint64_t)lastBlock);

    if (state != HAL_OK) {
        return false;
    }

    return HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER;
}

bool SDIOClass::readCID(void* cid) {
    uint32_t s[4];
    s[0] = __builtin_bswap32(hsd.CID[0]);
    s[1] = __builtin_bswap32(hsd.CID[1]);
    s[2] = __builtin_bswap32(hsd.CID[2]);
    s[3] = __builtin_bswap32(hsd.CID[3]);
  memcpy(cid, &s, 16);
  return true;
}

bool SDIOClass::readCSD(void* csd) {
    uint32_t s[4];
    s[0] = __builtin_bswap32(hsd.CSD[0]);
    s[1] = __builtin_bswap32(hsd.CSD[1]);
    s[2] = __builtin_bswap32(hsd.CSD[2]);
    s[3] = __builtin_bswap32(hsd.CSD[3]);
  memcpy(csd, &s, 16);
  return true;
}

uint32_t SDIOClass::cardSize() {
return	(hsd.SdCard.BlockNbr);

}

uint8_t SDIOClass::errorCode() {
  return m_errorCode;
}

uint32_t SDIOClass::errorData() {
  return 0;
}

uint32_t SDIOClass::errorLine() {
  return m_errorLine;
}

void SDIOClass::useDMA(bool useDMA){
	_useDMA = useDMA;
}

/*
 * DMA Handler declared here to replace the weak one in the core.
 * The name is derived from the Stream name.
 */
extern "C" {
void SDIO_DMA_IRQHandler(SDIO_Stream)(void) {
    reinterpret_cast<class SDIOClass*>(_sdio_this)->_sdioDMACallback();
}
void SDIO_IRQHandler(void){
    reinterpret_cast<class SDIOClass*>(_sdio_this)->_sdioCallback();
}
}

