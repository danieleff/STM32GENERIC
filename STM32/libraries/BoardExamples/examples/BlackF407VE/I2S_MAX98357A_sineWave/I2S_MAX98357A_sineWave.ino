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

// SPI2 pins are used for I2S.
// SPI2 is avalailable on the BalckF407VE and does not conflict with peripherals like SDIO
I2SClass I2S(SPI2, PC3 /*DIN*/ , PB12 /*LRC*/, PB10 /*SCLK*/);

#define SAMPLINGFREQUENCY 44100
#define WAVEBUFFERLENGTH  SAMPLINGFREQUENCY * 1 // 1 seconds

#define DAC_DC_VALUE      0x4000 // DAC value when there is no sound amplitude
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

void playBuffer(int16_t *monoBuffer, uint32_t bufferLength)
{
  uint32_t n;
  for ( n = 0; n < bufferLength; n++ )
  {
    int16_t dacValue = wave_i16[ n ] + DAC_DC_VALUE ;

    // write left channel
    I2S.write( dacValue );

    // write right channel
    // mono amplifier, right channel could be anything
    // we just write the same value as left channel
    // just in case you want to connect a stereo I2S-DAC
    I2S.write( dacValue );
  }
}

void loop()
{
  playBuffer(wave_i16,WAVEBUFFERLENGTH);
  delay( 3000 ); // delay next playing
}
