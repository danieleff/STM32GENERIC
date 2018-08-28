/*
  I2S sine wave modulation

  Show the use of the buffered I2S interface.
  This example produces a digital sine wave which is frequency modulated by a seconsd oscillator on the I2S interface of the
  microcontroller.

  

  Hardware:
   microcontroller board: STM32F407VE (  512MB flash )
                          STM32F407VG ( 1024MB flash )

   amplifier:             Adafruit I2S amplifier with the MAX98357a chip
                          https://www.adafruit.com/product/3006

  Pin description

  sd  = DIN  // I2S data
  ws  = LRC  // left/right channel
  ck  = SCLK // serial data clock

  MARCH 2018 ChrisMicro

*/
#include "I2S.h"

// SPI2 pins are used for I2S.
// SPI2 is avalailable on the BalckF407VE and does not conflict with peripherals like SDIO
I2SClass I2S(SPI2, PB15 /*SD-DIN*/ , PB12 /*WS-LRC*/, PB13 /*CK-SCLK*/);
//I2SClass I2S(SPI2, PC3 /*SD-DIN*/ , PB12 /*WS-LRC*/, PB10 /*CK-SCLK*/);

#define SAMPLINGFREQUENCY 44100

#define DAC_MAX_AMPLITUDE 0x2000 // experimental maximum on my speakers without distortion

#define SAMPLEBUFFERLENGTH 128
int16_t SampleBuffer[SAMPLEBUFFERLENGTH];

void setup()
{
  I2S.begin(I2S_PHILIPS_MODE, SAMPLINGFREQUENCY, 16);
}

float CarrierFrequency = 440;

// slow update oscillator
void updateControl()
{
  static float frequency = 100;
  frequency *= 1.003;
  if (frequency > 1600)frequency = 100;
  CarrierFrequency = frequency;
}

// fast main oscillator
int updateAudio()
{
  int out;

  const float amplitude = DAC_MAX_AMPLITUDE;

  static float phase = 0;
  float dphase = 2 * PI * CarrierFrequency / SAMPLINGFREQUENCY;

  out = sin(phase) * amplitude;
  phase += dphase;
  if (phase > 2 * PI)phase -= 2 * PI;

  return out;
}

void loop()
{
  int n;
  while (n < SAMPLEBUFFERLENGTH)
  {
    int16_t val = updateAudio();
    
    // the MAX98357a is only single channel. Writting both channels increases the resulting loudness
    SampleBuffer[n++] = val; // left
    SampleBuffer[n++] = val; // right
  }

  I2S.write(SampleBuffer, SAMPLEBUFFERLENGTH);
  updateControl();

}
