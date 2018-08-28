## I2C

To use the hardware I2C, include it in your code:
```c++
#include "Wire.h"
```

The first I2C object is explicitly created, and is connected to the I2C1 instance.

**Please check your board documentation to see which pins is I2C connected to by default.**

To use additional I2C, use the constructors to create your object, or the stm32SetInstance() method.

#### **`begin()`**

Start the I2C in master mode with default 100Khz speed.

#### **`begin(uint8_t address)`**

Start the I2C in slave mode with default 100Khz speed. Slave mode currently does not work in F1 chips.

#### **`beginTransmission(uint8_t address)`**

Set the address for the next transmission.

#### **`write(uint8_t data)`**

Master mode: writes the data into a buffer to be send out in `endTransmission`.

Slave mode: Send response to master. Currently can only be used inside the onRequest callback.

#### **`endTransmission()`**

Send the buffer to the slave.

#### **`uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)`**

Requests data from the slave. Returns the number of bytes read into the receive buffer.

The data then can be retrieved using `read()`. 

#### **`int available()`**

Returns data available in the receive buffer.

#### **`int read()`**

Returns the next byte in the receive buffer, or -1, if no more data available.

#### **`SetClock(uint32_t frequency)`**

Set the I2C clock frequency, and reinitializes the I2C peripheral.

#### **`onReceive(void (*callback)(int))`**

Sets the callback function in slave mode. When data is received, this will be called. 

The current implementation calls this for every byte received.

#### **`onRequest(void (*callback)(void)) `**

Sets the callback function in slave mode. When data is requested, this will be called.

You can respond to the master using the `write()` method.

#### **`stm32SetSDA(uint8_t pin);`**

Set the SDA pin used by this I2C.

**This method must be called before begin()!**

#### **`stm32SetSCL(uint8_t pin);`**

Set the alternative SCL pin used by this I2C.

**This method must be called before begin()!**

#### **`stm32SetInstance(I2C_TypeDef *instance);`**

Set the alternative I2C instance (I2C1/I2C2/...) used by this object.

**This method must be called before begin()!**

> Example: If you want to use a library that has hardcoded `I2C` in it, but you want to use I2C2:

```c++
#include "Wire.h"

void setup() {
    Wire.stm32SetInstance(I2C2);
    Wire.stm32SetSDA(sda);
    Wire.stm32SetSCL(scl);
    ...
    library.begin();
}

```

