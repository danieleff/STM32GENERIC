#if defined(STM32F1) || defined(STM32F2) || defined(STM32F4) || defined(STM32L1)

#include "stm32_gpio_af.h"
#include "SDIO.h"

#include "Arduino.h"

uint8_t SDIOClass::begin() {
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_SDIO_CLK_ENABLE();

    stm32AfSDIO4BitInit(SDIO, NULL, 0, NULL, 0,
            NULL, 0, NULL, 0, NULL, 0, NULL, 0);

    handle.Instance = SDIO;
    handle.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    handle.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    handle.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    handle.Init.BusWide = SDIO_BUS_WIDE_1B;
    handle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;

    handle.Init.ClockDiv = 0;

    error = HAL_SD_Init(&handle, &SDCardInfo);
    if (error != SD_OK) {
        return false;
    }

    error = HAL_SD_WideBusOperation_Config(&handle, SDIO_BUS_WIDE_4B);

    if (error != SD_OK) {
        return false;
    }

    return true;
}

#endif
