#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <Arduino.h>
#include "stm32_gpio_af.h"

#include "stm32_HAL/stm32XXxx_ll_spi.h"

#if defined(STM32F1) || defined(STM32F4)
#define SPI_HAS_OLD_DMATRANSFER
#endif

// SPI_HAS_EXTENDED_TRANSFER means SPI has
//   - transfer(uint8_t data, uint8_t *rxBuffer, size_t count)
//   - transfer(uint8_t *txBuffer, uint8_t *rxBuffer, size_t count)
#define SPI_HAS_EXTENDED_TRANSFER

// SPI_HAS_EXTENDED_NONBLOCKING_TRANSFER means SPI has
//   - implies SPI_HAS_EXTENDED_TRANSFER
//   - transfer(uint8_t data, uint8_t *rxBuffer, size_t count, callback)
//   - transfer(uint8_t *txBuffer, uint8_t *rxBuffer, size_t count, callback)
#define SPI_HAS_EXTENDED_NONBLOCKING_TRANSFER

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

typedef void (*spi_callback_type)();

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

    SPIClass(uint8_t mosi, uint8_t miso, uint8_t sck) {
        stm32SetMOSI(mosi);
        stm32SetMISO(miso);
        stm32SetSCK(sck);

        spiHandle.Instance = stm32GetSPIInstance(mosiPort, mosiPin, misoPort, misoPin, sckPort, sckPin);
    }

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

    void setDataWidth16(bool width16) {
        if (width16) {
            LL_SPI_SetDataWidth(spiHandle.Instance, LL_SPI_DATAWIDTH_16BIT);

            hdma_spi_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
            hdma_spi_rx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;

            hdma_spi_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
            hdma_spi_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;

        } else if (LL_SPI_GetDataWidth(spiHandle.Instance) == LL_SPI_DATAWIDTH_16BIT) {
            LL_SPI_SetDataWidth(spiHandle.Instance, LL_SPI_DATAWIDTH_8BIT);

            hdma_spi_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
            hdma_spi_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;

            hdma_spi_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
            hdma_spi_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        }
    }


    bool transfer(uint8_t data, uint8_t *rxBuffer, size_t count) {
        repeatTransmitData = data;
        if (transfer((uint8_t*) NULL, rxBuffer, count, NULL)) {
            flush();
            return true;
        }
        return false;
    }
    bool transfer(uint8_t *txBuffer, uint8_t *rxBuffer, size_t count) {
        if (transfer(txBuffer, rxBuffer, count, NULL)) {
            flush();
            return true;
        }
        return false;
    }
    bool transfer(uint8_t data, uint8_t *rxBuffer, size_t count, spi_callback_type callback) {
        repeatTransmitData = data;
        return transfer((uint8_t*) NULL, rxBuffer, count, callback);
    }
    bool transfer(uint8_t *txBuffer, uint8_t *rxBuffer, size_t count, spi_callback_type callback);


    bool transfer16(uint16_t data, uint16_t *rxBuffer, size_t count) {
        setDataWidth16(true);

        repeatTransmitData = data;

        return transfer((uint8_t*)NULL, (uint8_t*)rxBuffer, count);
    }
    bool transfer16(uint16_t *txBuffer, uint16_t *rxBuffer, size_t count) {
        setDataWidth16(true);

        return transfer((uint8_t*)txBuffer, (uint8_t *)rxBuffer, count);
    }
    bool transfer16(uint16_t data, uint16_t *rxBuffer, size_t count, spi_callback_type callback) {
        setDataWidth16(true);

        repeatTransmitData = data;
        return transfer((uint8_t*) NULL, (uint8_t*)rxBuffer, count, callback);
    }
    bool transfer16(uint16_t *txBuffer, uint16_t *rxBuffer, size_t count, spi_callback_type callback) {
        setDataWidth16(true);

        return transfer((uint8_t*)txBuffer, (uint8_t*)rxBuffer, count, callback);
    }

    void flush(void);
    bool done(void);

	uint8_t __attribute__ ((deprecated)) dmaTransfer(uint8_t *transmitBuf, uint8_t *receiveBuf, uint16_t length);
	uint8_t __attribute__ ((deprecated)) dmaSend(uint8_t *transmitBuf, uint16_t length, bool minc = 1);


    SPI_HandleTypeDef spiHandle = {};

    uint16_t repeatTransmitData = 0XFFFF;
    spi_callback_type callback;

    volatile bool dmaDone = true;

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
}

inline uint16_t SPIClass::transfer16(uint16_t data) {
    LL_SPI_SetDataWidth(spiHandle.Instance, LL_SPI_DATAWIDTH_16BIT);

    while(__HAL_SPI_GET_FLAG(&spiHandle, SPI_FLAG_TXE) == RESET);

    *(volatile uint16_t*)&spiHandle.Instance->DR = data;

    while(__HAL_SPI_GET_FLAG(&spiHandle, SPI_FLAG_RXNE) == RESET);
    while(__HAL_SPI_GET_FLAG(&spiHandle, SPI_FLAG_BSY) == SET);

    uint16_t ret = *(volatile uint16_t*)&spiHandle.Instance->DR;

    LL_SPI_SetDataWidth(spiHandle.Instance, LL_SPI_DATAWIDTH_8BIT);

    return ret;
}

inline void SPIClass::transfer(uint8_t *buf, size_t count) {
	HAL_SPI_TransmitReceive(&spiHandle, buf, buf, count, 1000);
}

extern SPIClass SPI;

#endif
