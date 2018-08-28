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

#### **`endTransaction();`**

#### **`uint8_t transfer(uint8_t data);`**

Send a 8 bits on SPI, and return the received 8 bit data.

#### **`uint16_t transfer16(uint16_t data);`**

Send a 16 bits on SPI, and return the received 16 bit data.

#### **`transfer(uint8_t *buf, size_t count);`**

Send the data in the buffer. The received data is stored in the same buffer.

#### **`transfer(tx, rxBuffer, size_t count[, callback]);`**

`tx`|
---              |---
uint8_t txData   | the same byte is sent `count` times
uint8_t *txBuffer| the buffer is sent
NULL             | 0xFF is sent `count` times

`rxBuffer`|
---               |---
uint8_t *rxBuffer | the received data is stored in the rxBuffer
NULL              | the received data is ignored

`callback`|
---                | ---
parameter not set  | transfer is blocking
NULL               | transfer is **non-blocking**, sends the data using DMA, and returns immediately.
function pointer   | transfer is **non-blocking**, sends the data using DMA, and returns immediately.  At the end of the transfer, the callback function is called in interrupt.

#### **`void flush(void);`**

Waits for the last non-blocking transfer to complete.

#### **`bool done(void);`**

Returns true if the last non-blocking transfer is completed.

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

