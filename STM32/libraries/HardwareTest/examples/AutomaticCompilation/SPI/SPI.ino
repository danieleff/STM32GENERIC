//Automatic testing compilation of basic SPI API

#include "SPI.h"

void setup() {
    SPI.begin();
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
    
    SPI.transfer(5);
    
    SPI.endTransaction();
    SPI.end();
}

void loop() {

}
