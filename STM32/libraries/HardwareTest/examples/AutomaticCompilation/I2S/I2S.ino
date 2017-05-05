//Automatic testing compilation of basic SPI API

#include "I2S.h"

I2SClass i2s(SPI1, PA0, PA1, PA2);

void setup() {
    i2s.begin(I2S_PHILIPS_MODE, 44000, 16);
    i2s.write(1);
}

void loop() {

}
