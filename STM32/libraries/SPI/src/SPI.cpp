#include "SPI.h"

#include "variant.h"

#include "stm32_dma.h"

static uint8_t spi_ff_buffer = 0XFF;

#if defined(MOSI) || defined(MISO) || defined(SCK)
	SPIClass SPI(SPI1, MOSI, MISO, SCK);
#else
	SPIClass SPI(SPI1);
#endif



void SPIClass::begin() {

	apb_freq = stm32GetClockFrequency((void*)spiHandle.Instance);

	spiHandle.Init.Mode = SPI_MODE_MASTER;
	spiHandle.Init.Direction = SPI_DIRECTION_2LINES;
	spiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
	spiHandle.Init.NSS = SPI_NSS_SOFT;

	__HAL_RCC_DMA1_CLK_ENABLE();
#ifdef __HAL_RCC_DMA2_CLK_ENABLE
	__HAL_RCC_DMA2_CLK_ENABLE();
#endif


#ifdef SPI1
        if (spiHandle.Instance== SPI1) {
            __HAL_RCC_SPI1_CLK_ENABLE();
        }
    #endif
    #ifdef SPI2
        else if (spiHandle.Instance == SPI2) {
            __HAL_RCC_SPI2_CLK_ENABLE();
        }
    #endif
    #ifdef SPI3
        else if (spiHandle.Instance == SPI3) {
            __HAL_RCC_SPI3_CLK_ENABLE();
        }
    #endif
    #ifdef SPI4
        else if (spiHandle.Instance ==  SPI4) {
            __HAL_RCC_SPI4_CLK_ENABLE();
        }
    #endif
    #ifdef SPI5
        else if (spiHandle.Instance ==  SPI5) {
            __HAL_RCC_SPI5_CLK_ENABLE();
        }
    #endif
    #ifdef SPI6
        else if (spiHandle.Instance ==  SPI6) {
            __HAL_RCC_SPI6_CLK_ENABLE();
        }
    #endif


    //////////////// DMA

    stm32DmaAcquire(&hdma_spi_tx, SPI_TX, spiHandle.Instance, true);
    stm32DmaAcquire(&hdma_spi_rx, SPI_RX, spiHandle.Instance, true);

	hdma_spi_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_spi_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_spi_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_spi_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_spi_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_spi_tx.Init.Mode = DMA_NORMAL;
	hdma_spi_tx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	_SPISetDMAFIFO(hdma_spi_tx);

	//hdma_spi_rx.Init.Channel = _ChannelRX;
	hdma_spi_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_spi_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_spi_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_spi_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_spi_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_spi_rx.Init.Mode = DMA_NORMAL;
	hdma_spi_rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	_SPISetDMAFIFO(hdma_spi_rx);

	__HAL_LINKDMA(&spiHandle, hdmatx, hdma_spi_tx);
	__HAL_LINKDMA(&spiHandle, hdmarx, hdma_spi_rx);

	stm32AfSPIInit(spiHandle.Instance, mosiPort, mosiPin, misoPort, misoPin, sckPort, sckPin);

}

void SPIClass::beginTransaction(SPISettings settings) {
	if (this->settings.clock == settings.clock
			&& this->settings.bitOrder == settings.bitOrder
			&& this->settings.dataMode == settings.dataMode) {
		return;
	}
	this->settings = settings;

	if (settings.clock >= apb_freq / 2) {
		spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	} else if (settings.clock >= apb_freq / 4) {
		spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	} else if (settings.clock >= apb_freq / 8) {
		spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	} else if (settings.clock >= apb_freq / 16) {
		spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	} else if (settings.clock >= apb_freq / 32) {
		spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	} else if (settings.clock >= apb_freq / 64) {
		spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	} else if (settings.clock >= apb_freq / 128) {
		spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	}  else {
		spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	}

	if(settings.bitOrder == MSBFIRST) {
		spiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
	} else {
		spiHandle.Init.FirstBit = SPI_FIRSTBIT_LSB;
	}

	if((settings.dataMode == SPI_MODE0) || (settings.dataMode == SPI_MODE1)) {
		spiHandle.Init.CLKPolarity     = SPI_POLARITY_LOW;
	} else {
		spiHandle.Init.CLKPolarity     = SPI_POLARITY_HIGH;
	}

	if((settings.dataMode == SPI_MODE0) || (settings.dataMode == SPI_MODE2)) {
		spiHandle.Init.CLKPhase        = SPI_PHASE_1EDGE;
	} else {
		spiHandle.Init.CLKPhase        = SPI_PHASE_2EDGE;
	}

	HAL_SPI_Init(&spiHandle);
	__HAL_SPI_ENABLE(&spiHandle);
}

void SPIClass::end() {
	//TODO deinit GPIO
	HAL_DMA_Init(&hdma_spi_tx);
	HAL_DMA_Init(&hdma_spi_rx);
}

void SPIClass::endTransaction() {

}
void SPIClass::setBitOrder(uint8_t bitOrder) {
	beginTransaction(SPISettings(settings.clock, bitOrder, settings.dataMode));
}
void SPIClass::setDataMode(uint8_t dataMode) {
	beginTransaction(SPISettings(settings.clock, settings.bitOrder, dataMode));
}
void SPIClass::setClockDivider(uint8_t clockDevider) {
	beginTransaction(SPISettings(apb_freq / clockDevider, settings.bitOrder, settings.dataMode));
}

void SPIClass::stm32SetMOSI(uint8_t mosi) {
	mosiPort = variant_pin_list[mosi].port;
	mosiPin = variant_pin_list[mosi].pin_mask;
}

void SPIClass::stm32SetMISO(uint8_t miso) {
	misoPort = variant_pin_list[miso].port;
	misoPin = variant_pin_list[miso].pin_mask;
}

void SPIClass::stm32SetSCK(uint8_t sck) {
	sckPort = variant_pin_list[sck].port;
	sckPin = variant_pin_list[sck].pin_mask;
}

void SPIClass::stm32SetInstance(SPI_TypeDef *instance) {
	spiHandle.Instance = instance;
}

#if defined(STM32F1) || defined(STM32F4)

uint8_t SPIClass::dmaTransfer(uint8_t *transmitBuf, uint8_t *receiveBuf, uint16_t length) {
	//HAL_SPI_TransmitReceive(&spiHandle, transmitBuf, receiveBuf, length, 1000);
	// DMA handles configured in Begin.
	if (length == 0) return 0;

    #ifdef STM32F1
	    __HAL_DMA_DISABLE(&hdma_spi_tx);
	    __HAL_DMA_DISABLE(&hdma_spi_rx);
    #endif

	if (!transmitBuf) {
		transmitBuf = &spi_ff_buffer;
		hdma_spi_tx.Init.MemInc = DMA_MINC_DISABLE;
	} else {
		//Need to change the MINC mode since dmaSend with MINC 0 or Null transmitBuf may have been called last
		hdma_spi_tx.Init.MemInc = DMA_MINC_ENABLE;
	}

	HAL_DMA_Init(&hdma_spi_tx);
	HAL_DMA_Init(&hdma_spi_rx);

	HAL_SPI_TransmitReceive_DMA(&spiHandle, transmitBuf, receiveBuf, length);

	while (spiHandle.State != HAL_SPI_STATE_READY);

	return 0;
}
uint8_t SPIClass::dmaSend(uint8_t *transmitBuf, uint16_t length, bool minc) {

    #ifdef STM32F1
        __HAL_DMA_DISABLE(&hdma_spi_tx);
    #endif

	//HAL_SPI_TransmitReceive(&spiHandle, transmitBuf, buf, length, 1000);
	//Need to set TX DMA handle.
	if (minc == 1){
		hdma_spi_tx.Init.MemInc = DMA_MINC_ENABLE;
	} else {
		hdma_spi_tx.Init.MemInc = DMA_MINC_DISABLE;
	}

	HAL_DMA_Init(&hdma_spi_tx);

	HAL_SPI_Transmit_DMA(&spiHandle, transmitBuf, length);

	while (spiHandle.State != HAL_SPI_STATE_READY);

	return 0;
}

extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    // Called at the end of DMA
}

#endif
