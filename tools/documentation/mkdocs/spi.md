## SPI

To use the hardware SPI, include it in your code:
```c++
#include "SPI.h"
```

The first SPI object is explicitly created, and is connected to the SPI1 instance.

**Please check your board documentation to see which pins is SPI connected to by default.**

To use additional SPI, use the constructors to create your object, or the stm32SetInstance() method.

#### **`SPIClass(SPI_TypeDef *instance)`**

Create a SPI object that is connected to the instance, on the default mosi/miso/sck. 
To avoid confusion, it is better to use the constructor below with implicit pins.

#### **`SPIClass(SPI_TypeDef *instance, uint8_t mosi, uint8_t miso, uint8_t sck)`**

Create a SPI object that is connected to the instance, on the specified mosi/miso/sck pins.

Please check the documentation of the chip to see which pins can be used.

> Example: set up SPI2

```c++
SPIClass SPI_2(SPI2, mosi, iso, sck);
void setup() {
    SPI_2.begin();
}

```

#### **`begin()`**

Enables SPI on the pins

#### **`end()`**

Disables SPI on the pins

#### **`beginTransaction(SPISettings settings);`**

Initializes SPI with the provided `SPISettings mySetting(speedMaximum, dataOrder, dataMode)` parameters.

Parameter|Description
---         |---
speedMaximum| The maximum speed requested
dataOrder   | `MSBFIRST`, `LSBFIRST` 
dataMode    | `SPI_MODE0`, `SPI_MODE1`, `SPI_MODE2`, or `SPI_MODE3` 

Example:

```c++
SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
```

TODO disable interrupt in pin if `usingInterrupt` was used

#### **`endTransaction();`**

TODO enable interrupt in pin if `usingInterrupt` was used

#### **`uint8_t transfer(uint8_t data);`**

Send a 8 bits on SPI, and return the received 8 bit data.

#### **`uint16_t transfer16(uint16_t data);`**

Send a 16 bits on SPI, and return the received 16 bit data.

#### **`transfer(uint8_t *buf, size_t count);`**

Send the data in the buffer. The received data is stored in the same buffer.

#### **`usingInterrupt();`**

TODO not yet implemented

#### **`uint8_t dmaSend(uint8_t *transmitBuf, uint16_t length, bool minc = true);`**

Send data using DMA in **blocking mode**.

* minc: false - only send the first byte of the `transmitBuf`, `length` times
* minc: true - send `length` bytes from the `transmitBuf`

#### **`uint8_t dmaTransfer(uint8_t *transmitBuf, uint8_t *receiveBuf, uint16_t length);`**

Send and receive data using DMA in **blocking mode**.

If `transmitBuf` is NULL, `0xFF` is transmitted instead.

#### **`stm32SetMOSI(uint8_t pin);`**

Set the MOSI pin used by this SPI.

**This method must be called before begin()!**

#### **`stm32SetMISO(uint8_t pin);`**

Set the alternative MISO pin used by this SPI.

**This method must be called before begin()!**

#### **`stm32SetSCK(uint8_t pin);`**

Set the alternative SCK pin used by this SPI.

**This method must be called before begin()!**

#### **`stm32SetInstance(SPI_TypeDef *instance);`**

Set the alternative SPI instance (SPI1/SPI2/...) used by this object.

**This method must be called before begin()!**

> Example: If you want to use a library that has hardcoded `SPI` in it, but you want to use SPI2:

```c++
#include "SPI.h"

void setup() {
    SPI.stm32SetInstance(SPI2);
    SPI.stm32SetMOSI(mosi);
    SPI.stm32SetMISO(miso);
    SPI.stm32SetSCK(sck);
    ...
    library.begin();
}

```

