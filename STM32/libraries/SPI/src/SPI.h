#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <Arduino.h>
#include "stm32_gpio_af.h"

#if defined(STM32F1) || defined(STM32F4)
#define SPI_HAS_OLD_DMATRANSFER
#endif

// SPI_HAS_TRANSACTION means SPI has
//   - beginTransaction()
//   - endTransaction()
//   - usingInterrupt()
//   - SPISetting(clock, bitOrder, dataMode)
#define SPI_HAS_TRANSACTION 1

// SPI_HAS_EXTENDED_CS_PIN_HANDLING means SPI has automatic
// CS pin handling and provides the following methods:
//   - begin(pin)
//   - end(pin)
//   - setBitOrder(pin, bitorder)
//   - setDataMode(pin, datamode)
//   - setClockDivider(pin, clockdiv)
//   - transfer(pin, data, SPI_LAST/SPI_CONTINUE)
//   - beginTransaction(pin, SPISettings settings) (if transactions are available)
// #define SPI_HAS_EXTENDED_CS_PIN_HANDLING 1

// For compatibility with sketches designed for AVR @ 16 MHz
// need to go from 64MHz to 16 (/4)
// New programs should use SPI.beginTransaction to set the SPI clock
#define SPI_CLOCK_DIV2	 2
#define SPI_CLOCK_DIV4	 4
#define SPI_CLOCK_DIV8	 8
#define SPI_CLOCK_DIV16	 16
#define SPI_CLOCK_DIV32	 32
#define SPI_CLOCK_DIV64	 64
#define SPI_CLOCK_DIV128 128

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

#if defined(STM32F4) || defined(STM32F7)
	#define _SPISetDMAFIFO(hdma_handler)	do { hdma_handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE; \
								hdma_handler.Init.FIFOMode = DMA_FIFOMODE_ENABLE; \
								hdma_handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL; \
								hdma_handler.Init.MemBurst = DMA_MBURST_SINGLE; \
								hdma_handler.Init.PeriphBurst = DMA_PBURST_SINGLE; } while (0)

#else
    #define _SPISetDMAFIFO(hdma_handler)
#endif

class SPISettings {
  public:
    SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode): clock(clock), bitOrder(bitOrder), dataMode(dataMode) {};
    SPISettings(): clock(0), bitOrder(0), dataMode(0) {};
    //init(4000000, MSBFIRST, SPI_MODE0);

    uint32_t clock;
    uint8_t bitOrder;
    uint8_t dataMode;
};



class SPIClass {
  public:
    SPIClass(SPI_TypeDef *instance) {
    	spiHandle.Instance = instance;
    };
    SPIClass(SPI_TypeDef *instance, uint8_t mosi, uint8_t miso, uint8_t sck) {
		spiHandle.Instance = instance;

		stm32SetMOSI(mosi);
		stm32SetMISO(miso);
		stm32SetSCK(sck);
	};

    void stm32SetMOSI(uint8_t mosi);
    void stm32SetMISO(uint8_t miso);
    void stm32SetSCK(uint8_t sck);
    void stm32SetInstance(SPI_TypeDef *instance);

    void begin();
    void end();

    void beginTransaction(SPISettings settings);
	void endTransaction();

	void setBitOrder(uint8_t);
	void setDataMode(uint8_t);
	void setClockDivider(uint8_t);

    uint8_t transfer(uint8_t data);
    uint16_t transfer16(uint16_t data);
    void transfer(uint8_t *buf, size_t count);
	uint8_t dmaTransfer(uint8_t *transmitBuf, uint8_t *receiveBuf, uint16_t length);
	uint8_t dmaSend(uint8_t *transmitBuf, uint16_t length, bool minc = 1);


    SPI_HandleTypeDef spiHandle = {};
  private:
    uint32_t apb_freq = 0;

    SPISettings settings = {};

    DMA_HandleTypeDef hdma_spi_rx = {};
    DMA_HandleTypeDef hdma_spi_tx = {};

    GPIO_TypeDef *mosiPort = NULL;
    uint32_t mosiPin = 0;
    GPIO_TypeDef *misoPort = NULL;
    uint32_t misoPin = 0;
    GPIO_TypeDef *sckPort = NULL;
    uint32_t sckPin = 0;
};

inline uint8_t SPIClass::transfer(uint8_t data) {
    while(__HAL_SPI_GET_FLAG(&spiHandle, SPI_FLAG_TXE) == RESET);

	*(volatile uint8_t*)&spiHandle.Instance->DR = data;

	while(__HAL_SPI_GET_FLAG(&spiHandle, SPI_FLAG_RXNE) == RESET);
	while(__HAL_SPI_GET_FLAG(&spiHandle, SPI_FLAG_BSY) == SET);

	return *(volatile uint8_t*)&spiHandle.Instance->DR;

	/*
	if (HAL_SPI_TransmitReceive(&spiHandle, &data, &data, 1, 1000) != HAL_OK) {
		return 0;
	}
	return data;
	*/
}
inline uint16_t SPIClass::transfer16(uint16_t data) {
	if (HAL_SPI_TransmitReceive(&spiHandle, (uint8_t*)&data, (uint8_t*)&data, 2, 1000) != HAL_OK) {
		return 0;
	}
	return data;
}

inline void SPIClass::transfer(uint8_t *buf, size_t count) {
	HAL_SPI_TransmitReceive(&spiHandle, buf, buf, count, 1000);
}

extern SPIClass SPI;

#endif
