//Automatic testing compilation of basic SDIO API

#include "SDIO.h"

SDIOClass sdio;

void setup() {
    sdio.begin();
    uint8_t data[512];
    
    sdio.readBlocks(1, data, 1);
}

void loop() {

}
