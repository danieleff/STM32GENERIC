/*
  Test and print statistics of the onboard SPI-flash memmory

  board:
    BLACK407VE

  how to:
  1. Install library: SPIFlash by Prajwal Bhattaram (Marzogh)
    https://github.com/Marzogh/SPIFlash

  2. Upload sketch
  3. Open USB serial monitor

  Issues: Start the serial monitor in less than 5 seconds after downloading
          The USB-serial may hang due to the amount of data printed.

*/

#include<SPIFlash.h>

SPIFlash flash(PB0);

void setup()
{
  SPI.stm32SetMOSI(PB5);
  SPI.stm32SetMISO(PB4);
  SPI.stm32SetSCK(PB3);

  Serial.begin(115200);
  delay(5000);
  Serial.println("starting to test external FLASH");

  flash.begin();

  randomSeed(65535537);
}

void loop() {
  for (int i = 4; i > 0; i--) {
    Serial.print(".");
    delay(1000);
  }

  getID();
  diagnose();
  Serial.println("END");

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Serial Print Functions~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void clearprintBuffer(char *bufPtr)
{
  for (uint8_t i = 0; i < 128; i++) {
    //printBuffer[i] = 0;
    *bufPtr++ = 0;
  }
}

void printLine() {
  Serial.println();
}

void printPass() {
  Serial.print(F("Pass"));
}

void printFail() {
  Serial.print(F("Fail"));
}

void printTab(uint8_t a, uint8_t b) {
  for (uint8_t i = 0; i < a; i++) {
    Serial.print(F("\t"));
  }
  if (b > 0) {
    Serial.print("||");
    for (uint8_t i = 0; i < b; i++) {
      Serial.print(F("\t"));
    }
  }
}

void printTime(uint32_t _wTime, uint32_t _rTime) {
  printTab(2, 1);
  printTimer(_wTime);
  printTab(2, 1);
  printTimer(_rTime);
}

void printTimer(uint32_t _us) {

  if (_us > 1000000) {
    float _s = _us / (float)1000000;
    Serial.print(_s, 4);
    Serial.print(" s");
  }
  else if (_us > 10000) {
    float _ms = _us / (float)1000;
    Serial.print(_ms, 4);
    Serial.print(" ms");
  }
  else {
    Serial.print(_us);
    Serial.print(F(" us"));
  }
}