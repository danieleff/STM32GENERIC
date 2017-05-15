/*
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !! important: read installation description below !!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   SD-Card test for the BlackF407VE

   - read card info and print it to the serial monitor
   - write test.txt file to card
   - read it and print it on the serial monitor


   1. Install SdFat-beta from https://github.com/greiman/SdFat-beta
   By the way you can install SPIFlash from sketch / library / manage libraries

   2. In SdFatConfig.h, Change ENABLE_SDIO_CLASS 0 to 1
   If you have installed the library with the library manager in windows your will find the
   file here:
   C:\Users\..WhatEverUser..\Documents\Arduino\libraries\SdFat\src

   sdFatConfig.h:
   ..
   // Enable SDIO driver if available.
   #if defined(__MK64FX512__) || defined(__MK66FX1M0__)
   #define ENABLE_SDIO_CLASS 1
   #define ENABLE_SDIOEX_CLASS 1
   #else  // ENABLE_SDIO_CLASS
   #define ENABLE_SDIO_CLASS 0  // ==> !! change this to ENABLE_SDIO_CLASS 1 !!
   #endif  // ENABLE_SDIO_CLASS
  ...
*/

#include "Arduino.h"
#include "SDIO.h"
#include "STM32SdFatSdio.h"

STM32SdFatSdio sd;

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 5; i++) {
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

  for (int i = 0; i < 5; i++) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();

}