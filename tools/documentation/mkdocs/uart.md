## Serial UART

There are two implementations of Serial communication: UART and USB. 

This page describes the UART based.

Please check the [Menu option](menu_options#automatic-serial) to see which one is selected.

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
    SerialUART2.stm32SetRX(Pcd);
    SerialUART2.stm32SetTX(Pab);
    SerualUART2.begin(115200);
}
void loop() {
    int data = SerialUART2.read();
    if (data != -1) {
        SerialUART2.write(data);
    }
}
```

