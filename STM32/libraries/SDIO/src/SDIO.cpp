//TODO add 1bit/4bit/8bit mode selection in begin()
//TODO add pin set functions
//TODO add instance set function and constructor
//TODO test compile to all variants

#include "stm32_gpio_af.h"
#include "SDIO.h"

#include "Arduino.h"

SD_HandleTypeDef hsd;
HAL_SD_CardInfoTypedef SDCardInfo;

uint8_t SDIOClass::begin() {
    GPIO_InitTypeDef GPIO_InitStruct;

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

    error = HAL_SD_Init(&hsd, &SDCardInfo);
    if (error != SD_OK) {
        return false;
    }

    error = HAL_SD_WideBusOperation_Config(&hsd, SDIO_BUS_WIDE_4B);

    if (error != SD_OK) {
        return false;
    }

    return true;
}

uint8_t SDIOClass::readBlocks(uint32_t block, uint8_t* dst, size_t nb) {
    error = HAL_SD_ReadBlocks(&hsd, (uint32_t*)dst, (uint64_t)block * 512, 512, nb);

    if (error != SD_OK) {
        return false;
    }

    return HAL_SD_GetStatus(&hsd) == SD_TRANSFER_OK;

}

uint8_t SDIOClass::writeBlocks(uint32_t block, const uint8_t* src, size_t nb) {
    error = HAL_SD_WriteBlocks(&hsd, (uint32_t*)src, (uint64_t)block * 512, 512, nb);

    if (error != SD_OK) {
        return false;
    }

    return HAL_SD_GetStatus(&hsd) == SD_TRANSFER_OK;
}
