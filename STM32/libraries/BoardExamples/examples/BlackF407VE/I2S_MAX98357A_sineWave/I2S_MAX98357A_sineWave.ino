/*
  I2S sine wave

  This example produces a digital sine wave on the I2S interface of the
  microcontroller.

  Hardware:
   microcontroller board: STM32F407VE Black
   amplifier:             Adafruit I2S amplifier with the MAX98357a chip
                          https://www.adafruit.com/product/3006

  Pin description

  sd  = DIN  // I2S data
  ws  = LRC  // left/right channel
  ck  = SCLK // serial data clock

  MAY 2017 ChrisMicro

*/
#include "I2S.h"

// to use the example check your board schematic
// if this is the correct pin setting for your board.
// general setup without master clock
I2SClass I2S(SPI3, PC12 /*DIN*/ , PA4 /*LRC*/, PC10 /*SCLK*/);

#define SAMPLINGFREQUENCY 44100
#define WAVEBUFFERLENGTH  SAMPLINGFREQUENCY * 1 // 1 seconds

#define DAC_DC_VALUE      0x4000
#define DAC_MAX_AMPLITUDE 0x2000 // experimental maximum on my speakers without distortion

int16_t wave_i16[WAVEBUFFERLENGTH];

void setup()
{
  I2S.begin(I2S_PHILIPS_MODE, SAMPLINGFREQUENCY, 16);

  float   frequency = 440;
  float   amplitude = DAC_MAX_AMPLITUDE;
  // calculate buffer values for test tone
  for ( int n = 0; n < WAVEBUFFERLENGTH; n++ ) wave_i16[n] = ( sin( 2 * PI * frequency / SAMPLINGFREQUENCY * n ) ) * amplitude;
}

uint32_t BufferIndex = 0;

void loop()
{
  // write left channel
  I2S.write( wave_i16[ BufferIndex++ ] + DAC_DC_VALUE );
  // mono amplifier, right channel not available, write zeros
  I2S.write( 0 );

  if ( BufferIndex > WAVEBUFFERLENGTH)
  {
    delay( 3000 );
    BufferIndex = 0;
  }
}

