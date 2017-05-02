//Automatic testing compilation of basic I2C API

#include "Wire.h"

void setup() {
    Wire.begin();
    Wire.beginTransmission(4);
    Wire.write(6);
    Wire.endTransmission();
    Wire.end();
}

void loop() {

}
