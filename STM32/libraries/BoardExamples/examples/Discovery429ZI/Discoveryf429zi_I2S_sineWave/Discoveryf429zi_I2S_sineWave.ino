/*
 I2S sine wave for STM32f429 Discovery

 This example produces a sine  on the I2S interface for the 
 microcontroller. 
 
 For this example you might use a e.g an Adafruit I2S amplifier with the MAX98357a chip to produce sound
 https://www.adafruit.com/product/3006
 
 Pin description

 sd  = DIN  // I2S data
 ws  = LRC  // left/right channel 
 ck  = SCLK // serial data clock 

 Attention:
 This example works only when not using the TFT.
 Unfortunatelly the I2S3 Pins are shared with the TFT pins of the board.

 PA4:  I2S3_WS, TFT-VSYNC
 PC10: I2S3_CK, TFT-RGB
 PC12: I2S3_SD, TFT-RGB

 June 2018 ChrisMicro
 
 */
#include "I2S.h"

// to use the example check your board schematic
// if this is the correct pin setting for your board.
I2SClass I2S(SPI3, PC12 /*sd*/ , PA4/*ws*/, PC10 /*ck*/);


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
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop()
{
  digitalWrite(LED_BUILTIN,HIGH);
  for (int n = 0; n < NUMBEROFSAMPLES; n++) 
  {
    I2S.write(Buffer[n]); //left
    I2S.write(Buffer[n]); //right
  }
  digitalWrite(LED_BUILTIN,LOW);
  delay(3000);
}
