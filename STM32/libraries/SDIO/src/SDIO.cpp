//TODO add 1bit/4bit/8bit mode selection in begin()
//TODO add pin set functions
//TODO add instance set function and constructor
//TODO test compile to all variants

#define DBGPRNT     Serial.print ("State value (HAL_SD_STATE...): "); \
                    Serial.println (hsd.State, HEX); \
                    Serial.print ("Errorcode value (HAL_SD_ERROR...): "); \
                    Serial.println (hsd.ErrorCode, HEX); \
                    Serial.print ("R1 value: "); \
                    Serial.println (hsd.Instance->RESP1, BIN); \
                    Serial.print ("HAL Return value (HAL_...): "); \
                    Serial.println (state, HEX);\
                    Serial.println (__FILE__);\
                    Serial.println (__LINE__);\
                    Serial.println (__func__);


#include "stm32_gpio_af.h"
#include "SDIO.h"

#include "stm32_dma.h"

#include "Arduino.h"

static SDIOClass *_sdio_this;

//=============================================================================
// Error function and macro.
#define sdError(code) setSdErrorCode(code, __LINE__)
inline bool setSdErrorCode(uint8_t code, uint32_t line) {
  m_errorCode = code;
  m_errorLine = line;
  return false;  // setSdErrorCode
}

uint32_t SDMMC_CmdSetWrtBlkEraseCount(SDIO_TypeDef *SDIOx, uint32_t Count){
    SDIO_CmdInitTypeDef  sdmmc_cmdinit;
    uint32_t errorstate = SDMMC_ERROR_NONE;

    //First send command 55, then ACMD.

    /* Pre-erase Count blocks once Write is received */
    sdmmc_cmdinit.Argument         = (uint32_t)Count;
    sdmmc_cmdinit.CmdIndex         = 23;
    sdmmc_cmdinit.Response         = SDIO_RESPONSE_SHORT;
    sdmmc_cmdinit.WaitForInterrupt = SDIO_WAIT_NO;
    sdmmc_cmdinit.CPSM             = SDIO_CPSM_ENABLE;
    SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);

    /* Check for error conditions */
    //errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_READ_MULT_BLOCK, SDIO_CMDTIMEOUT);
    errorstate = SDIO_GetResponse(SDIOx, SDIO_RESP1);
    if((errorstate & SDMMC_OCR_ERRORBITS) == SDMMC_ALLZERO){
        return HAL_OK;
    }
    return errorstate;
}

uint32_t SDMMC_CmdSwitchFunction(SDIO_TypeDef *SDIOx, uint32_t arg){
    /*
     * Todo: Finish this function, not finished yet, needs to read 64bytes and drop them.
     */
    SDIO_CmdInitTypeDef  sdmmc_cmdinit;
    uint32_t errorstate = SDMMC_ERROR_NONE;

    //First send command 55, then ACMD.

    /* Pre-erase Count blocks once Write is received */
    sdmmc_cmdinit.Argument         = (uint32_t)arg;
    sdmmc_cmdinit.CmdIndex         = 6;
    sdmmc_cmdinit.Response         = SDIO_RESPONSE_SHORT;
    sdmmc_cmdinit.WaitForInterrupt = SDIO_WAIT_NO;
    sdmmc_cmdinit.CPSM             = SDIO_CPSM_ENABLE;
    SDIO_SendCommand(SDIOx, &sdmmc_cmdinit);

    /* Check for error conditions */
    //errorstate = SDMMC_GetCmdResp1(SDIOx, SDMMC_CMD_READ_MULT_BLOCK, SDIO_CMDTIMEOUT);
    errorstate = SDIO_GetResponse(SDIOx, SDIO_RESP1);
    if((errorstate & SDMMC_OCR_ERRORBITS) == SDMMC_ALLZERO){
        return HAL_OK;
    }
    return errorstate;
}

uint32_t SDIOClass::cardStatus(){
    HAL_SD_GetCardState(&hsd);
    return SDIO_GetResponse(hsd.Instance, SDIO_RESP1);
}


uint8_t SDIOClass::begin() {
    //GPIO_InitTypeDef GPIO_InitStruct;
    if (hsd.State == HAL_SD_STATE_READY){
        /*
         * TODO: Check card state, may not be in transfer mode.
         */
        return true;
    }

    _sdio_this = this;

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
        DBGPRNT;
        return false;
    }

    __HAL_RCC_DMA2_CLK_ENABLE();
    stm32DmaAcquire(&hdma_sdio, SDIO_RXTX, SDIO, true);

    hdma_sdio.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdio.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;

#ifdef DMA_PFCTRL
    hdma_sdio.Init.Mode = DMA_PFCTRL;
#else
    hdma_sdio.Init.Mode = DMA_NORMAL;
#endif

    hdma_sdio.Init.Priority = DMA_PRIORITY_LOW;
    _SDIOSetDMAFIFO(hdma_sdio);

    __HAL_LINKDMA(&hsd, hdmatx, hdma_sdio);
    __HAL_LINKDMA(&hsd, hdmarx, hdma_sdio);

    HAL_NVIC_SetPriority(SDIO_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SDIO_IRQn);

    if (HAL_DMA_Init(&hdma_sdio) != HAL_OK) {
        return false;
    }

    /*
     * Up to here
     */
    _useDMA = true;

    m_errorCode = 0; //SD_CARD_ERROR_NONE;
    return true;
}
/* TODO: This part needs more work, should set the card to idle state
 *
 */
uint8_t SDIOClass::end() {
    SDMMC_CmdGoIdleState(hsd.Instance);
    return (HAL_SD_DeInit(&hsd) == HAL_OK);
}

uint8_t SDIOClass::readBlocks(uint32_t block, uint8_t* dst, size_t nb) {
    bool aligned = ((uint32_t)dst & 0x3U) == 0;

    if (!_useDMA || !aligned) {
        state = HAL_SD_ReadBlocks(&hsd, dst, block, nb, (uint32_t)sd_timeout);

        if (state != HAL_OK) {
            DBGPRNT;
            return false;
        }

        //common with dma, so it's down at the end
        //return HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER;
    }
    else {
        hdma_sdio.Init.Direction = DMA_PERIPH_TO_MEMORY;
        if (HAL_DMA_Init(&hdma_sdio) != HAL_OK) {
            return false;
        }
        state = HAL_SD_ReadBlocks_DMA(&hsd, dst, block, nb);
        if (state != HAL_OK) {
            DBGPRNT;
            return false;
        }
        // We need to block here, until we implement a callback feature
        uint32_t tickstart = HAL_GetTick();
        while (hsd.State == HAL_SD_STATE_BUSY){
            if((HAL_GetTick() - tickstart) >=  sdRdTimeout * nb)
            {
                /* Abort transfer and send return error */
                HAL_SD_Abort(&hsd);
                DBGPRNT;
                Serial.print ("Timeout on Read, over ");
                Serial.print (sdRdTimeout * nb);
                Serial.println ("ms");
                return false;
            }
        }
        if (hsd.State != HAL_SD_STATE_READY ) {
            DBGPRNT;
            return false;
        }
        if (__HAL_SD_GET_FLAG(&hsd,SDIO_FLAG_DCRCFAIL)) {
           //return false;
            DBGPRNT;
            Serial.println ("CRC error on read");
            while (1); //stay here
        }
    }
    return HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER;
}

uint8_t SDIOClass::writeBlocks(uint32_t block, const uint8_t* src, size_t nb) {
    bool aligned = ((uint32_t)src & 0x3U) == 0;

    uint32_t tickstart = HAL_GetTick();
    while (!(cardStatus() & SDCARD_STATUS_READY_BIT)) {
        SDMMC_CmdStopTransfer(hsd.Instance);
        if ((HAL_GetTick() - tickstart) > sdBsyTimeout) {
            DBGPRNT;
            return false;
        }
        yield();
    }
    /*
    if (nb > 1){
        if(SDMMC_CmdAppCommand(hsd.Instance, (uint32_t)(hsd.SdCard.RelCardAdd << 16U)) != SDMMC_ERROR_NONE)
        {
            return false;
        }
        SDMMC_CmdSetWrtBlkEraseCount(hsd.Instance,nb);
    }
    */

    if (!_useDMA || !aligned) {
        state = HAL_SD_WriteBlocks(&hsd,(uint8_t*) src, block, (uint32_t) nb, sd_timeout);

        if (state != HAL_OK) {
            DBGPRNT;
            return false;
        }
//common with dma, so it's down at the end
        //return HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER;
    }
    else {
        hdma_sdio.Init.Direction = DMA_MEMORY_TO_PERIPH;
        if (HAL_DMA_Init(&hdma_sdio) != HAL_OK) {
            return false;
        }
        state = HAL_SD_WriteBlocks_DMA(&hsd, (uint8_t*)src, block, nb);
        if (state != HAL_OK) {
            DBGPRNT;
            return false;
        }
        /*
         *  We need to block here, until we implement a callback feature
         *  TODO: better check if the card is still in programming mode.
         *  Also confirm the timeout to wait from the specs, currently sd_timeout
         *  It may need to be multiplied by a factor depending on number of blocks.
         *  Finally, perhaps rename sd_timeout to something else
         */
        tickstart = HAL_GetTick();
        while (hsd.State == HAL_SD_STATE_BUSY){
            if((HAL_GetTick() - tickstart) >=  sdWrTimeout * (nb+1))
            {
                /* Abort transfer and send return error */
                HAL_SD_Abort(&hsd);
                DBGPRNT;
                Serial.print ("Timeout on write, over ");
                Serial.print (sdWrTimeout * (nb+1));
                Serial.println ("ms");
                return false;
            }
        }
        if (hsd.State != HAL_SD_STATE_READY) {
            DBGPRNT;
            return false;
        }
    }
    if (__HAL_SD_GET_FLAG(&hsd,SDIO_FLAG_DCRCFAIL)) {
       //return false;
        DBGPRNT;
        Serial.println ("CRC failed on Write");
        while (1); //stay here
    }
        while (HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_PROGRAMMING);
        return HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER;
}

bool SDIOClass::erase(uint32_t firstBlock, uint32_t lastBlock) {
    state = HAL_SD_Erase(&hsd, (uint64_t)firstBlock, (uint64_t)lastBlock);

    if (state != HAL_OK) {
        DBGPRNT;
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


extern "C" void SDIO_IRQHandler() {
    HAL_SD_IRQHandler(&(_sdio_this->hsd));
}
