/*
 I2S square wave for Nucleo F401

 This example produces a digital sawtooth on the I2S interface for the 
 microcontroller. It produces a continous sqaure wave.
 
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

void setup()
{
  I2S.begin(I2S_PHILIPS_MODE, 44000, 16);  
}

#define DAC_MAX_AMPLITUDE 32000/8; // experimental maximum on my speakers without distortion

void loop()
{
  int16_t amplitudeLeft = DAC_MAX_AMPLITUDE; 
  int16_t amplitudeRight = DAC_MAX_AMPLITUDE; 

  int n;

  // output frequency = 44000Hz/100 = 440Hz
  // rectangle low part ( 50 samples low )
  for(n=0;n<50;n++)
  {
    I2S.write(-amplitudeLeft); 
    I2S.write(-amplitudeRight); 
  }
  // rectangle high part ( 50 samples hight )
  for(n=0;n<50;n++)
  {
    I2S.write(amplitudeLeft); 
    I2S.write(amplitudeRight);   
  }

}

