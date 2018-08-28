## I2S

Not to be confused with [I2C](i2c.md), is a protocol for connecting to audio devices.
The samples are sent continuously in the background using DMA transfer. 

## **This library is in beta, and the API / implementation is subject to change!**

To use this library, include it in your code:

```c++
#include "I2S.h"

I2SClass i2s(...);

```

#### **`I2SClass(SPI_TypeDef *instance);`**

Construct I2S that will be connected to the specified I2S instance on chip.

The board default pins will be used for sd/ws/ck. mck will not be used.

#### **`I2SClass(SPI_TypeDef *instance, uint8_t sd, uint8_t ws, uint8_t ck);`**

Construct I2S that will be connected to the specified I2S instance on chip.

The specified pins will be used for sd/ws/ck. mck will not be used.

#### **`I2SClass(SPI_TypeDef *instance, uint8_t sd, uint8_t ws, uint8_t ck, uint8_t mck);`**

Construct I2S that will be connected to the specified I2S instance on chip.

The specified pins will be used for sd/ws/ck/mck.

#### **`uint8_t I2SClass::begin(i2s_mode_t mode, uint32_t sampleRate, uint8_t bitsPerSample);`**

Configure the pins, and the parameters for the connection.

* mode: one of I2S_PHILIPS_MODE,I2S_RIGHT_JUSTIFIED_MODE, I2S_LEFT_JUSTIFIED_MODE
* sampleRate: between 8000 and 96000
* bitsPerSample: 16. ** (Note: 24 or 32 bit per sample currently not working!) **

#### **`write(int16_t data);`**

Send 16 bit data. Blocks if there is no space left in the internal buffer.

#### **`write(int16_t *data, uint16_t size);`**

Send 16 bit data. Blocks if there is no space left in the internal buffer.

#### **`setBuffer(uint16_t *buffer, int bufferSize);`**

Set the DMA transfer buffer size. The default buffer is 2048 * 16 bits.

#### **`int getBufferSize();`**

Returns size of the transfer buffer.

#### **`stm32SetSD(uint8_t pin);`**

Set the alternative SD pin used by this I2S.

**This method must be called before begin()!**

#### **`stm32SetWD(uint8_t pin);`**

Set the alternative WD pin used by this I2S.

**This method must be called before begin()!**

#### **`stm32SetCK(uint8_t pin);`**

Set the alternative CK pin used by this I2S.

**This method must be called before begin()!**

#### **`stm32SetMCK(uint8_t pin);`**

Set the alternative MCK pin used by this I2S.

**This method must be called before begin()!**


