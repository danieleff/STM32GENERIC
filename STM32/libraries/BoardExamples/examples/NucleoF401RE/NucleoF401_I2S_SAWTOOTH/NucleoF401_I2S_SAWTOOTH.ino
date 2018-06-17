/*
 I2S sawtooth for Nucleo F401

 This example produces a digital sawtooth on the I2S interface for the 
 microcontroller.

 If the microcontroller on your board supports an I2S interface ( not all do )
 you can use e.g an Adafruit I2S amplifier with the MAX98357a chip to produce sound
 https://www.adafruit.com/product/3006
 
 Pin description

 sd  = DIN  // I2S data
 ws  = LRC  // left/right channel 
 ck  = SCLK // serial data clock 
 MCK = MCLK // master clock, some I2S chips ( like the CS42L22 ) need fast external clock provided by the MCU       
 
 June 2018 ChrisMicro
 
 */
#include "I2S.h"

//definitions in variant.h for Nucleo F401
//#define LED_BUILTIN   PA5
//#define USER_BTN   PC13

// to use the example check your board schematic
// if this is the correct pin setting for your board.
// setup  with masterclock enabled
I2SClass I2S(SPI3, PC12 /*sd*/ , PA4 /*ws*/, PC10 /*ck*/, PC7/* MCK*/); 



void setup()
{
  Serial.begin(115200);
  I2S.begin(I2S_PHILIPS_MODE, 44000, 16);
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(USER_BTN, INPUT);

  digitalWrite(LED_BUILTIN, HIGH);
  
}


void loop()
{
  static int16_t dacValue;

//  uint16_t n;
  float f_Hz=440;
  float fs_Hz=44000;
  float amplitude=10000;
  float deltaPhase=2*PI*f_Hz/fs_Hz;
  static float phase=0;
  int n;
  int16_t sig;
  /*
  for(n=0;n<110;n++)
  {

    sig     = amplitude*sin(phase);
    phase  += deltaPhase;
    I2S.write(sig); // left channel
    //I2S.write(sig); // right channel

  }
  */
   // Serial.println(millis());

  Serial.println(millis());
  // write I2S buffer for continous sound
  // even during serial out
  /*
  for(n=0;n<220;n++)
  {
    dacValue+=300; // saw tooth
    I2S.write(dacValue); // left channel
    I2S.write(dacValue); // right channel
  }*/
  
  dacValue=-30000;
  for(n=0;n<50;n++)
  {
    I2S.write(0); // left channel
    I2S.write(dacValue); // left channel

  }
  dacValue=30000;
  for(n=0;n<50;n++)
  {
    I2S.write(0); // left channel
    I2S.write(dacValue); // left channel
  }

/*  
  Serial.println(millis());

  if(digitalRead(USER_BTN))
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);
*/
}

