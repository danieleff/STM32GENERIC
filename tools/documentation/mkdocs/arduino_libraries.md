# Standard libraries:

## Serial

There are two implementations of Serial: UART and USB.

TODO: IMPLEMENT You can set what is Serial from the boards menu. By default, it is aligned with the upload method to help debugging.

* If the board is Nucleo, automatic Serial is aliased to [SerialUART2](Serial UART) (ST-Link Virtual COM port).
* If the upload method is based on UART (Serial upload, ST-Link), automatic Serial is aliased to [SerialUART1](Serial UART).
* If the upload method is based on USB (DFU), automatic Serial aliased to [SerialUSB](Serial USB).

## Serial UART

**`SerialUART1`**is bound to UART1, **`SerialUART2`** to UART2 etc...

Please refer to the [official documentation](https://www.arduino.cc/en/reference/serial) for
the description of Serial functions.

#### **`SerialUARTx.stm32SetRX(uint8_t pin)`**

Set the RX signal to the specified pin. Please check the chip documentation for which pin can be used for RX.
This must be called before begin()!

#### **`SerialUARTx.stm32SetTx(uint8_t pin)`**

Set the TX signal to the specified pin. Please check the chip documentation for which pin can be used for TX.
This must be called before begin()!


> Example: The following code will echo the received bytes on UART2
```c++
void setup() {
    SerialUART2.set_rx(Pcd);
    SerialUART2.set_tx(Pab);
    SerualUART2.begin(115200);
}
void loop() {
    int data = SerialUART2.read();
    if (data != -1) {
        SerialUART2.write(data);
    }
}
```

## Serial USB

If the [USB menu](#USB) is set to include Serial, SerialUSB is set as a virtual COM port, and can be used just like a regular UART based.

TODO explain windows 7 driver

Please refer to the [official documentation](https://www.arduino.cc/en/reference/serial) for
the description of Serial functions.

## SPI

The first SPI, connected to SPI1 instance.
To use additional SPI, use the constructors to create your object, or the stm32_set_instance() method.

#### **`SPIClass(SPI_TypeDef *instance)`**

Create a SPI object that is connected to the instance, on the default mosi/miso/sck. 
To avoid confusion, it is better to use the constructor below with implicit pins.

#### **`SPIClass(SPI_TypeDef *instance, uint8_t mosi, uint8_t miso, uint8_t sck)`**

Create a SPI object that is connected to the instance, on the specified mosi/miso/sck pins.

> Example: set up SPI2
`SPIClass SPI_2(SPI2, mosi, iso, sck);`

#### **`SPI.begin()`**

Configures the SPI pins to the 

#### **`SPI.end()`**

Disables the SPI

#### **`SPI.beginTransaction(...);`**

TODO write doc

#### **`SPI.endTransaction(...);`**

TODO write doc

#### **`SPI.transfer(...);`**

TODO write doc

#### **`SPI.transfer16(...);`**

TODO write doc

#### **`SPI.transfer(...);`**

TODO write doc

#### **`SPI.usingInterrupt();`**

TODO not yet implemented

#### **`SPI.transfer(...);`**

TODO write doc

#### **`SPI.stm32SetMOSI(uint8_t pin);`**

TODO write doc
This method must be called before begin()!

#### **`SPI.stm32SetMISO(uint8_t pin);`**

TODO write doc
This method must be called before begin()!

#### **`SPI.stm32SetSCK(uint8_t pin);`**

TODO write doc
This method must be called before begin()!

#### **`SPI.stm32SetInstance(...);`**

TODO implement
This method must be called before begin()!

>Example: TODO example for using libraries on multiple instances with the same SPI


## Wire - I2C

TODO write doc

# USB

TODO set USB in menu (show used pins, + mini PB9)

