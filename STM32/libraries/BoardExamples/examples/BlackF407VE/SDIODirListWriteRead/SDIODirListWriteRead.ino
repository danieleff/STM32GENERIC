// 1. Install SdFat-beta from https://github.com/greiman/SdFat-beta
// 2. In SdFatConfig.h, Change ENABLE_SDIO_CLASS 0 to 1


#include "Arduino.h"
#include "SDIO.h"
#include "STM32SdFatSdio.h"

STM32SdFatSdio sd;

void setup() {
    Serial.begin(115200);

    for(int i=0; i<5; i++) {
        delay(1000);
        Serial.print(".");
    }
}

void loop() {
    Serial.println("Starting");
    delay(1000);

    if (!sd.begin()) {
      Serial.println("Card error");
      return;
    }

    ///////////////

    Serial.println("LS:");
    sd.ls(LS_R | LS_DATE | LS_SIZE);

    ///////////////

    File file;

    Serial.println("Writing:");
    file = sd.open("test.txt", FILE_WRITE);
    if (file) {
        file.print("Testing write\n");
        file.close();
    } else {
        Serial.print("Error opening file");
    }

    ///////////////

    Serial.println("Reading:");
    file = sd.open("test.txt");
    if (file) {
        while (file.available()) {
          Serial.write(file.read());
        }
        file.close();
    } else {
        Serial.print("Error opening file");
    }

    ///////////////

    for(int i=0; i<5; i++) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();

}
