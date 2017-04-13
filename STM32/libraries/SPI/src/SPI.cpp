#include "SPI.h"

SPIClass SPI(SPI1);

void SPIClass::begin() {
	apb_freq = stm32_apb_clock_freq((void*)spiHandle.Instance);

	spiHandle.Init.Mode = SPI_MODE_MASTER;
	spiHandle.Init.Direction = SPI_DIRECTION_2LINES;
	spiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
	spiHandle.Init.NSS = SPI_NSS_SOFT;

	#ifdef SPI1
		if (spiHandle.Instance == SPI1) __HAL_RCC_SPI1_CLK_ENABLE();
	#endif
	#ifdef SPI2
		if (spiHandle.Instance == SPI2) __HAL_RCC_SPI2_CLK_ENABLE();
	#endif
	#ifdef SPI3
		if (spiHandle.Instance == SPI3) __HAL_RCC_SPI3_CLK_ENABLE();
	#endif
	#ifdef SPI4
		if (spiHandle.Instance == SPI4) __HAL_RCC_SPI4_CLK_ENABLE();
	#endif
	#ifdef SPI5
		if (spiHandle.Instance == SPI5) __HAL_RCC_SPI5_CLK_ENABLE();
	#endif
	#ifdef SPI6
		if (spiHandle.Instance == SPI6) __HAL_RCC_SPI6_CLK_ENABLE();
	#endif

	stm32_af_spi_init(spiHandle.Instance, mosiPort, mosiPin, misoPort, misoPin, sckPort, sckPin);
};

void SPIClass::beginTransaction(SPISettings settings) {
	if (this->settings.clock == settings.clock
			&& this->settings.bitOrder == settings.bitOrder
			&& this->settings.dataMode == settings.dataMode) {
		return;
	}
	this->settings = settings;

	if (settings.clock > apb_freq / 2) {
		spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	} else if (settings.clock > apb_freq / 4) {
		spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	} else if (settings.clock > apb_freq / 8) {
		spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	} else if (settings.clock > apb_freq / 16) {
		spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	} else if (settings.clock > apb_freq / 32) {
		spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	} else if (settings.clock > apb_freq / 64) {
		spiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	} else if (settings.clock > apb_freq / 128) {
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

void SPIClass::stm32_set_mosi(uint8_t mosi) {
	mosiPort = port_pin_list[mosi].port;
	mosiPin = port_pin_list[mosi].pin_mask;
}

void SPIClass::stm32_set_miso(uint8_t miso) {
	misoPort = port_pin_list[miso].port;
	misoPin = port_pin_list[miso].pin_mask;
}

void SPIClass::stm32_set_sck(uint8_t sck) {
	sckPort = port_pin_list[sck].port;
	sckPin = port_pin_list[sck].pin_mask;
}
