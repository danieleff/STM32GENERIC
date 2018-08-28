## Overview

* STM32GENERIC uses standard C and C++, so control sturctures (if, for), data types(int, long), syntax, etc... work as expected.
* Non microcontroller-specific Arduino utility functions such as isLowerCase(), bitRead() etc... also work.
* All non-standard functions are prefixed with `stm32XXXX()`

#### **`uint32_t millis();`**

Returns the milliseconds since the microcontroller started. Overflows in 50 days.

#### **`uint32_t micros();`**

Returns the microseconds since the microcontroller started. Overflows in 70 minutes.

#### **`delay(uint32_t millis);`**

Delays the execution by the given milliseconds.

#### **`delayMicroseconds(uint32_t micros);`**

Delays the execution by the given microseconds.

> Example: Print the milliseconds on [Serial](arduino_api#serial), every second:

```c++
void setup() {
  Serial.begin(9600);
}
void loop() {
  Serial.println(millis());
  delay(1000);
}
```

> Uses STM32 SysTick, set to interrupt 1ms intervals.

Naming the pins in code:

* STM32 chip pins have names: `PA0`, `PA1`...
* On some boards (Nucleo, Maple), there are numbers / alternative names next to the pin: `1`, `RX`, `A0`.
* [Predefined constants](predefined_pin_constants) for primary LED, SPI, Serial, I2C are defined in the variant file for the specific board.

Generally you should stick to naming the pin the same as they appear on the board to reduce confusion. If on the board you see ABC, use digitalWrite(ABC, HIGH);

#### **`pinMode(uint8_t pin, uint8_t mode);`**

Sets the pin for digital reading / writing

Mode          | Meaning
--------------|--------
OUTPUT        | 0V - 3.3V
INPUT         | The pin does not have a pull down or pull up resistor
INPUT_PULLUP  | Weak pull up resistor to default to HIGH if the pin is not connected to anything
INPUT_PULLDOWN| Weak pull down resistor to default to LOW if the pin is not connected to anything

pinMode() is needed for digitalRead() and digitalWrite() functions.

The system automatically sets the pinMode when using a peripheral library like analogRead(), analogWrite(), [SPI](#spi) or [I2C](#wire-i2c), so you don't have to.

#### **`digitalRead(uint8_t pin);`**

Read the voltage level on the specified pin. Returns LOW(0) if it is 0V, HIGH(1) if it is 3.3V

**All pins can be used as digital input.**

Not all pins are 5V tolerant. Please check the chip reference manual `Pinouts and pin description` section.

#### **`digitalWrite(uint8_t pin, uint8_t value);`**

If value if LOW(0), sets the pin to 0V. If value is HIGH(1), 3.3V.
**All pins can be used as digital output.**

> Example: Blink the [LED](#predefined_pin_constants) on the board:

```c++
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(1000);
}
```

#### **`int analogRead(uint8_t pin);`**

Read the voltage level on the pin, and map the 0..3.3V range to 0..1023 range by default.

**Not all pins can be used as analog input!** Please check the [Boards page](boards.md).

#### **`analogReadResolution()`**

TODO not yet implemented

#### **`analogWrite(uint8_t pin, int value)`**

Set PWM to the specified pin.

**All pins can be used as analog/PWM output.**

> Uses the TIM2 clock interrupt with software PWM implementation to allow PWM on all pins.

#### **`analogWriteResolution()`**

TODO not yet implemented

> Example: Dim the LED based on the voltage value on PA0
```c++
void loop() {
    analogWrite(LED_BUILTIN, analogRead(PA0));
    delay(10);
}
```

#### **`void attachInterrupt(uint8_t, callback, int mode);`**

Setup callback on the specified pin. Mode can be: 
* CHANGE: When pin changes from LOW to HIGH, or HIGH to LOW, the callback will be called. 
* RISING: When pin changes from LOW to HIGH the callback will be called.
* FALLING: When pin changes from HIGH to LOW, the callback will be called.

Same pins from different ports cannot be used. For exaple PA3 and PB3 cannot be used as interrupt simultaneously.

### Predefined pin constants

The following constants are defined in the variant.h file for each variant:

Constant          | Meaning
------------------|-----------
LED_BUILTIN       | The primary LED on board, if there is any
STM32_LED_BUILTIN_ACTIVE_LOW | If set, the onboard LED lights up when output is set to LOW
MOSI              | MOSI pin of the first [SPI](spi.md)
MISO              | MISO pin of the first [SPI](spi.md)
SCK               | SCK  pin of the first [SPI](spi.md)
SDA               | SDA pin of the first [I2C](i2c.md)
SCL               | SCL pin of the first [I2C](i2c.md)
