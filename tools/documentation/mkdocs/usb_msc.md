## USB: Mass Storage

## **This library is in beta**

1. Select Mass Storage from the arduino tools/USB menu
2. Override the `BlockDevice.h` interface to provide implementation for reading / writing blocks
3. Include `MassStorage.h`, and implement the `BlockDevice *getMassStorage()` function

There is RAM implementation of BlockDevice, called RamBlockDevice. Example usage:

```c++
#include "Arduino.h"

#include "RamBlockDevice.h"

RamBlockDevice ramBlockDevice(100 /* x 512 blocks*/, true);

BlockDevice *getMassStorage() {
    return &ramBlockDevice;
}

void blinkOnRead(uint32_t block, size_t count) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void blinkOnWrite(uint32_t block, size_t count) {
    digitalWrite(LED1_BUILTIN, !digitalRead(LED1_BUILTIN));
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED1_BUILTIN, OUTPUT);

    ramBlockDevice.setReadListener(blinkOnRead);
    ramBlockDevice.setWriteListener(blinkOnWrite);
}

void loop() {

}
```
