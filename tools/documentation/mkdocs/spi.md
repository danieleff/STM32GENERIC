## SPI

The first SPI is connected to SPI1 instance.
To use additional SPI, use the constructors to create your object, or the stm32_set_instance() method.

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

#### **`transfer(...);`**

TODO write doc

#### **`transfer16(...);`**

TODO write doc

#### **`transfer(...);`**

TODO write doc

#### **`usingInterrupt();`**

TODO not yet implemented

#### **`transfer(...);`**

TODO write doc

#### **`stm32SetMOSI(uint8_t pin);`**

Set the MOSI pin used by this SPI.

**This method must be called before begin()!**

#### **`stm32SetMISO(uint8_t pin);`**

Set the MOSI pin used by this SPI.

**This method must be called before begin()!**

#### **`stm32SetSCK(uint8_t pin);`**

Set the MOSI pin used by this SPI.

**This method must be called before begin()!**

#### **`stm32SetInstance(SPI_TypeDef *instance);`**

Set the SPI instance (SPI1/SPI2/...) used by this object.

**This method must be called before begin()!**

> Example: If you want to use a library that has hardcoded `SPI` in it, but you want to use SPI2:

```c++
void setup() {
    SPI.stm32SetInstance(SPI2);
    SPI.stm32SetMOSI(mosi);
    SPI.stm32SetMISO(miso);
    SPI.stm32SetSCK(sck);
    ...
    library.begin();
}

```

