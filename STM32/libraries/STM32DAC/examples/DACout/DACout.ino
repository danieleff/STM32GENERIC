/*
  DACout.ino STM32DAC demo
  
  PA4_DACOUT and PA5_DACOUT are predefined instances of class STM32DAC.
  
  huaweiwx@sina.com 2018.7.21
*/

#include "STM32DAC.h"

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);
  PA4_DACOUT.Init();
}

// the loop routine runs over and over again forever:
void loop() {
  static uint16_t val = 0;
  if (++val > 0xfff) val = 0;
  PA4_DACOUT = val;
}

