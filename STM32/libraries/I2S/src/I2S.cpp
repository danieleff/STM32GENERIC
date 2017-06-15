#include "I2S.h"
#include "stm32_gpio_af.h"

#include "stm32_dma.h"

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

void I2SClass::setBuffer(int16_t *buffer, int bufferSize) {
    this->doubleBuffer = buffer;
    this->bufferSize = bufferSize;
}

int I2SClass::getBufferSize() {
    return bufferSize;
}

uint8_t I2SClass::begin(i2s_mode_t mode, uint32_t sampleRate, uint8_t bitsPerSample) {
    static int16_t static_buffer[2048];

    setBuffer(static_buffer, 2048);

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

    __HAL_RCC_DMA1_CLK_ENABLE();


    if (!stm32DmaAcquire(&dmaHandle, SPI_TX, (void*)handle.Instance, true)) {
        return false;
    }

    dmaHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
    dmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;
    dmaHandle.Init.MemInc = DMA_MINC_ENABLE;
    dmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    dmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    dmaHandle.Init.Mode = DMA_NORMAL;
    dmaHandle.Init.Priority = DMA_PRIORITY_LOW;
    dmaHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&dmaHandle);

    __HAL_LINKDMA(&handle,hdmatx,dmaHandle);

    return HAL_I2S_Init(&handle) == HAL_OK;
}

I2SClass *i2sDma;

extern "C" void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) {
    i2sDma->tail += i2sDma->dmaSendSize;

    if ((uint32_t)(i2sDma->head - i2sDma->tail) > i2sDma->dmaSendSize) {
        HAL_I2S_Transmit_DMA(hi2s, (uint16_t*)(i2sDma->doubleBuffer + (i2sDma->tail % i2sDma->bufferSize)), i2sDma->dmaSendSize);
    } else {
        i2sDma->dmaDone = true;
    }

}

void I2SClass::write(int16_t data) {
    while((head + 1) % bufferSize == tail % bufferSize);

    doubleBuffer[head % bufferSize] = data;
    head++;

    if (dmaDone && (uint32_t)(head - tail) > dmaSendSize) {
        i2sDma = this;
        dmaDone = false;
        HAL_I2S_Transmit_DMA(&handle, (uint16_t*)(doubleBuffer + (tail % bufferSize)), dmaSendSize);
    }

}

void I2SClass::write(int16_t *data, size_t samples) {
    for(size_t i=0; i<samples; i++) {
        write(data[i]);
    }
}

uint32_t I2SClass::availableForWrite() {
    return bufferSize - (head - tail);
}

uint32_t I2SClass::getDelay() {
    return dmaSendSize;
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
