/*
   SD-Card test for the BlackF407VE

   - read card info and print it to the serial monitor
   - write test.txt file to card
   - read it and print it on the serial monitor

*/

#include "Arduino.h"
#include "SdFat.h"

SdFatSdio sd;

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