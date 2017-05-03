#include "SPI.h"

#include "variant.h"

#if defined(MOSI) || defined(MISO) || defined(SCK)
	SPIClass SPI(SPI1, MOSI, MISO, SCK);
#else
	SPIClass SPI(SPI1);
#endif



void SPIClass::begin() {

	_DMA_Instance_Type *_StreamTX;
	_DMA_Instance_Type *_StreamRX;
	uint32_t _ChannelTX;
	uint32_t _ChannelRX;

	apb_freq = stm32GetClockFrequency((void*)spiHandle.Instance);

	spiHandle.Init.Mode = SPI_MODE_MASTER;
	spiHandle.Init.Direction = SPI_DIRECTION_2LINES;
	spiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
	spiHandle.Init.NSS = SPI_NSS_SOFT;
	spiHandle.hdmatx = &hdma_spi_tx;
	spiHandle.hdmarx = &hdma_spi_rx;

	__HAL_RCC_DMA1_CLK_ENABLE();
#ifdef __HAL_RCC_DMA2_CLK_ENABLE()
	__HAL_RCC_DMA2_CLK_ENABLE();
#endif


	#ifdef SPI1
		if (spiHandle.Instance== SPI1) {
			__HAL_RCC_SPI1_CLK_ENABLE();
			_StreamTX = SPIx_DMA(SPI1_StreamTX);
			_StreamRX = SPIx_DMA(SPI1_StreamRX);
			_ChannelTX = SPI1_ChannelTX;
			_ChannelRX = SPI1_ChannelRX;
			/*
			 * Not used yet, still just polling
			 */
			//_SPISetDmaIRQ(SPI1);
		}
	#endif
	#ifdef SPI2
		else if (spiHandle.Instance == SPI2) {
			__HAL_RCC_SPI2_CLK_ENABLE();
			_StreamTX = SPIx_DMA(SPI2_StreamTX);
			_StreamRX = SPIx_DMA(SPI2_StreamRX);
			_ChannelTX = SPI2_ChannelTX;
			_ChannelRX = SPI2_ChannelRX;
			/*
			 * Not used yet, still just polling
			 */
			//_SPISetDmaIRQ(SPI2);
		}
	#endif
	#ifdef SPI3
		else if (spiHandle.Instance == SPI3) {
			__HAL_RCC_SPI3_CLK_ENABLE();
			_StreamTX = SPIx_DMA(SPI3_StreamTX);
			_StreamRX = SPIx_DMA(SPI3_StreamRX);
			_ChannelTX = SPI3_ChannelTX;
			_ChannelRX = SPI3_ChannelRX;
			/*
			 * Not used yet, still just polling
			 */
			//_SPISetDmaIRQ(SPI3);
		}
	#endif
	#ifdef SPI4
		else if (spiHandle.Instance ==  SPI4) {
			__HAL_RCC_SPI4_CLK_ENABLE();
			_StreamTX = SPIx_DMA(SPI4_StreamTX);
			_StreamRX = SPIx_DMA(SPI4_StreamRX);
			_ChannelTX = SPI4_ChannelTX;
			_ChannelRX = SPI4_ChannelRX;
			/*
			 * Not used yet, still just polling
			 */
			//_SPISetDmaIRQ(SPI4);
		}
	#endif
	#ifdef SPI5
		else if (spiHandle.Instance ==  SPI5) {
			__HAL_RCC_SPI5_CLK_ENABLE();
			_StreamTX = SPIx_DMA(SPI5_StreamTX);
			_StreamRX = SPIx_DMA(SPI5_StreamRX);
			_ChannelTX = SPI5_ChannelTX;
			_ChannelRX = SPI5_ChannelRX;
			/*
			 * Not used yet, still just polling
			 */
			//_SPISetDmaIRQ(SPI5);
		}
	#endif
	#ifdef SPI6
		else if (spiHandle.Instance ==  SPI6) {
			__HAL_RCC_SPI6_CLK_ENABLE();
			_StreamTX = SPIx_DMA(SPI6_StreamTX);
			_StreamRX = SPIx_DMA(SPI6_StreamRX);
			_ChannelTX = SPI6_ChannelTX;
			_ChannelRX = SPI6_ChannelRX;
			/*
			 * Not used yet, still just polling
			 */
			//_SPISetDmaIRQ(SPI6);
		}
	#endif


	hdma_spi_tx.Instance = _StreamTX;
	hdma_spi_tx.Parent = &spiHandle;
	_SPISetDMAChannel(hdma_spi_tx, _ChannelTX);
	hdma_spi_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_spi_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_spi_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_spi_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_spi_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_spi_tx.Init.Mode = DMA_NORMAL;
	hdma_spi_tx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	_SPISetDMAFIFO(hdma_spi_tx);
/*
	hdma_spi_tx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	hdma_spi_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_spi_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_spi_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_spi_tx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_spi_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
*/

	hdma_spi_rx.Instance = _StreamRX;
	hdma_spi_rx.Parent = &spiHandle;
	_SPISetDMAChannel(hdma_spi_rx,_ChannelRX);
	//hdma_spi_rx.Init.Channel = _ChannelRX;
	hdma_spi_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_spi_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_spi_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_spi_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_spi_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_spi_rx.Init.Mode = DMA_NORMAL;
	hdma_spi_rx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
	_SPISetDMAFIFO(hdma_spi_rx);
	/*
	hdma_spi_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    hdma_spi_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_spi_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_spi_rx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_spi_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;
	*/

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
uint8_t SPIClass::dmaTransfer(uint8_t *transmitBuf, uint8_t *receiveBuf, uint16_t length) {
	//HAL_SPI_TransmitReceive(&spiHandle, transmitBuf, receiveBuf, length, 1000);
	// DMA handles configured in Begin.
	if (length == 0) return 0;
	if (!transmitBuf) {
		transmitBuf = &spi_ff_buffer;
		hdma_spi_tx.Init.MemInc = DMA_MINC_DISABLE;
	} else {
		//Need to change the MINC mode since dmaSend with MINC 0 or Null transmitBuf may have been called last
		hdma_spi_tx.Init.MemInc = DMA_MINC_ENABLE;
	}

	HAL_DMA_Init(&hdma_spi_tx);
	HAL_DMA_Init(&hdma_spi_rx);

	/*
	 * We dont use interrupts here.
	 */

	HAL_SPI_TransmitReceive_DMA(&spiHandle, transmitBuf, receiveBuf, length);
	HAL_DMA_PollForTransfer(&hdma_spi_tx, HAL_DMA_FULL_TRANSFER, 10000);
	HAL_DMA_PollForTransfer(&hdma_spi_rx, HAL_DMA_FULL_TRANSFER, 10000);
	HAL_DMA_IRQHandler(&hdma_spi_tx);
	HAL_DMA_IRQHandler(&hdma_spi_rx);
	spiHandle.State = HAL_SPI_STATE_READY;
	return 0;
}
uint8_t SPIClass::dmaSend(uint8_t *transmitBuf, uint16_t length, bool minc) {
	//HAL_SPI_TransmitReceive(&spiHandle, transmitBuf, buf, length, 1000);
	//Need to set TX DMA handle.
	if (minc == 1){
		hdma_spi_tx.Init.MemInc = DMA_MINC_ENABLE;
	} else {
		hdma_spi_tx.Init.MemInc = DMA_MINC_DISABLE;
	}

	HAL_DMA_Init(&hdma_spi_tx);

	HAL_SPI_Transmit_DMA(&spiHandle, transmitBuf, length);
	HAL_DMA_PollForTransfer(&hdma_spi_tx, HAL_DMA_FULL_TRANSFER, 10000);
	HAL_DMA_IRQHandler(&hdma_spi_tx);
	spiHandle.State = HAL_SPI_STATE_READY;
	return 0;
}
