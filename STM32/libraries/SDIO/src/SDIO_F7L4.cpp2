#if defined(STM32F7) || defined(STM32L4)

#include "stm32_gpio_af.h"
#include "SDIO.h"

#include "Arduino.h"

uint8_t SDIOClass::begin() {
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_SDMMC1_CLK_ENABLE();

    stm32AfSDIO4BitInit(SDMMC1, NULL, 0, NULL, 0,
            NULL, 0, NULL, 0, NULL, 0, NULL, 0);

    handle.Instance = SDMMC1;
    handle.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
    handle.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
    handle.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
    handle.Init.BusWide = SDMMC_BUS_WIDE_1B;
    handle.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;

    handle.Init.ClockDiv = 0;

    error = HAL_SD_Init(&handle, &SDCardInfo);
    if (error != SD_OK) {
        return false;
    }

    error = HAL_SD_WideBusOperation_Config(&handle, SDMMC_BUS_WIDE_4B);

    if (error != SD_OK) {
        return false;
    }

    return true;
}

#endif
