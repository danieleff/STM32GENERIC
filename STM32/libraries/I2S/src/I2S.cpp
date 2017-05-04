#include "I2S.h"
#include "stm32_gpio_af.h"

#include "Arduino.h"

I2SClass::I2SClass(SPI_TypeDef *instance) {
    handle.Instance = instance;
}

I2SClass::I2SClass(SPI_TypeDef *instance, uint8_t sd, uint8_t ws, uint8_t ck) {
    handle.Instance = instance;
    stm32SetSD(sd);
    stm32SetWS(ws);
    stm32SetCK(ck);
}

I2SClass::I2SClass(SPI_TypeDef *instance, uint8_t sd, uint8_t ws, uint8_t ck, uint8_t mck) {
    handle.Instance = instance;
    stm32SetSD(sd);
    stm32SetWS(ws);
    stm32SetCK(ck);
    stm32SetMCK(mck);
}

uint8_t I2SClass::begin(i2s_mode_t mode, uint32_t sampleRate, uint8_t bitsPerSample) {

    #ifdef SPI1
    if (handle.Instance == SPI1) __HAL_RCC_SPI1_CLK_ENABLE();
    #endif
    #ifdef SPI2
    if (handle.Instance == SPI2) __HAL_RCC_SPI2_CLK_ENABLE();
    #endif
    #ifdef SPI3
    if (handle.Instance == SPI3) __HAL_RCC_SPI3_CLK_ENABLE();
    #endif
    #ifdef SPI4
    if (handle.Instance == SPI4) __HAL_RCC_SPI4_CLK_ENABLE();
    #endif
    #ifdef SPI5
    if (handle.Instance == SPI5) __HAL_RCC_SPI5_CLK_ENABLE();
    #endif
    #ifdef SPI6
    if (handle.Instance == SPI6) __HAL_RCC_SPI6_CLK_ENABLE();
    #endif



    if (useMck) {
        stm32AfI2SInitWithMck(handle.Instance, sdPort, sdPin, wsPort, wsPin, ckPort, ckPin, mckPort, mckPin);
        handle.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
    } else {
        stm32AfI2SInit(handle.Instance, sdPort, sdPin, wsPort, wsPin, ckPort, ckPin);
        handle.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
    }

    if (mode == I2S_PHILIPS_MODE) {
        handle.Init.Standard = I2S_STANDARD_PHILIPS;
    } else if (mode == I2S_LEFT_JUSTIFIED_MODE) {
        handle.Init.Standard = I2S_STANDARD_LSB;
    } else if (mode == I2S_RIGHT_JUSTIFIED_MODE) {
        handle.Init.Standard = I2S_STANDARD_MSB;
    } else {
        return false;
    }

    handle.Init.Mode = I2S_MODE_MASTER_TX;
    handle.Init.AudioFreq = I2S_AUDIOFREQ_44K;
    handle.Init.CPOL = I2S_CPOL_LOW;
    handle.Init.ClockSource = I2S_CLOCK_PLL;

    #if defined(STM32F3) || defined(STM32F4)
    handle.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
    #endif

    if (sampleRate >= 96000) {
        handle.Init.AudioFreq = I2S_AUDIOFREQ_96K;
    } else if (sampleRate >= 48000) {
        handle.Init.AudioFreq = I2S_AUDIOFREQ_48K;
    } else if (sampleRate >= 44000) {
        handle.Init.AudioFreq = I2S_AUDIOFREQ_44K;
    } else if (sampleRate >= 32000) {
        handle.Init.AudioFreq = I2S_AUDIOFREQ_32K;
    } else if (sampleRate >= 22000) {
        handle.Init.AudioFreq = I2S_AUDIOFREQ_22K;
    } else if (sampleRate >= 16000) {
        handle.Init.AudioFreq = I2S_AUDIOFREQ_16K;
    } else if (sampleRate >= 11000) {
        handle.Init.AudioFreq = I2S_AUDIOFREQ_11K;
    } else {
        handle.Init.AudioFreq = I2S_AUDIOFREQ_8K;
    }

    if (bitsPerSample == 16) {
        handle.Init.DataFormat = I2S_DATAFORMAT_16B;
    } else if (bitsPerSample == 24) {
        handle.Init.DataFormat = I2S_DATAFORMAT_24B;
    } else if (bitsPerSample == 32) {
        handle.Init.DataFormat = I2S_DATAFORMAT_32B;
    } else {
        return false;
    }

    return HAL_I2S_Init(&handle) == HAL_OK;
}

void I2SClass::write(uint16_t data) {
    HAL_I2S_Transmit(&handle, &data, 1, 1000);
}

void I2SClass::write32(uint32_t data) {
    HAL_I2S_Transmit(&handle, (uint16_t*)&data, 1, 1000);
}

void I2SClass::write(uint16_t *data, uint16_t size) {
    HAL_I2S_Transmit(&handle, data, size, 1000);
}

void I2SClass::write32(uint32_t *data, uint16_t size) {
    HAL_I2S_Transmit(&handle, (uint16_t*)data, size, 1000);
}

void I2SClass::stm32SetSD(uint8_t sd) {
    this->sdPort = variant_pin_list[sd].port;
    this->sdPin = variant_pin_list[sd].pin_mask;
}

void I2SClass::stm32SetWS(uint8_t ws) {
    this->wsPort = variant_pin_list[ws].port;
    this->wsPin = variant_pin_list[ws].pin_mask;
}

void I2SClass::stm32SetCK(uint8_t ck) {
    this->ckPort = variant_pin_list[ck].port;
    this->ckPin = variant_pin_list[ck].pin_mask;
}

void I2SClass::stm32SetMCK(uint8_t mck) {
    this->mckPort = variant_pin_list[mck].port;
    this->mckPin = variant_pin_list[mck].pin_mask;
    useMck = true;
}
