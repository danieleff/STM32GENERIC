/*
 I2S sine wave for Nucleo F401

 This example produces a sine  on the I2S interface for the 
 microcontroller. 
 
 For this example you might use a e.g an Adafruit I2S amplifier with the MAX98357a chip to produce sound
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

#define SAMPLINGFREQUENCY 44100
#define NUMBEROFSAMPLES  SAMPLINGFREQUENCY * 1 // 1 seconds

#define DAC_MAX_AMPLITUDE 0x7FFF/8; // experimental maximum on my speakers without distortion

int16_t Buffer[NUMBEROFSAMPLES];

void setup()
{
  
  I2S.begin(I2S_PHILIPS_MODE, SAMPLINGFREQUENCY, 16);
  
  for (int n = 0; n < NUMBEROFSAMPLES; n++) 
  {
    const float   frequency = 440;
    const float   amplitude = DAC_MAX_AMPLITUDE;
    Buffer[n] = ( sin( 2 * PI * frequency / SAMPLINGFREQUENCY * n )) * amplitude;
  }
}

void loop()
{
  for (int n = 0; n < NUMBEROFSAMPLES; n++) 
  {
    I2S.write(Buffer[n]); //left
    I2S.write(Buffer[n]); //right
  }
  delay(3000);
}
