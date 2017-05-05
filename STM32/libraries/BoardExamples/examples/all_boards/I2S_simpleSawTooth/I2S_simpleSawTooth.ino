/*
 I2S sawtooth

 This example produces a digital sawtooth on the I2S interface for the 
 microcontroller.

 If the microcontroller on your board supports an I2S interface ( not all do )
 you can use e.g an Adafruit I2S amplifier with the MAX98357a chip to produce sound
 https://www.adafruit.com/product/3006
 
 Check your microcontroller if it has an I2S interface.
 E.g. the STM32F103C8 ( on the BluePill board ) has no I2S
 On this board you can use only certain chips like the
 PT8211 16 Bit Digital to Analog Converter togehter with the SPI interface

 Pin description

 sd  = DIN  // I2S data
 ws  = LRC  // left/right channel 
 ck  = SCLK // serial data clock 
 MCK = MCLK // master clock, some I2S chips ( like the CS42L22 ) need fast external clock provided by the MCU       
 
 April 2017 ChrisMicro
 
 */
#include "I2S.h"

// to use the example check your board schematic
// if this is the correct pin setting for your board.
// general setup without master clock
//I2SClass I2S(SPI3, PC12 /*sd*/ , PA4 /*ws*/, PC10 /*ck*/); 

// setup  with masterclock enabled
I2SClass I2S(SPI3, PC12 /*sd*/ , PA4 /*ws*/, PC10 /*ck*/, PC7/* MCK*/); 


void setup()
{
  I2S.begin(I2S_PHILIPS_MODE, 44000, 16);
}

void loop()
{
  static uint16_t dacValue;
  
  I2S.write(dacValue); 
  
  dacValue+=300; // saw tooth
}

