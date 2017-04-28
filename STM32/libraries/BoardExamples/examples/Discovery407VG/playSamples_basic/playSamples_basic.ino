/*

 STM32F4 Discovery sample player basics

 This program is just playing 2 samples from memory with random pitch
 and increasing speed.

 It lets room for a lot of creativity to make your own melodies.

 The samples are only 8bit@16kHz.
 16bit and higher frequency would give better sounds.

 April 2017, ChrisMicro
 
 */

#include "CS43L22_STM32.h"
#include "I2S.h"
#include "wavetables.h"

#define SAMPLINGFREQUENCY 16000

I2SClass I2S(SPI3, PC12 /*sd*/ , PA4 /*ws*/, PC10 /*ck*/, PC7/* MCK*/); // setup for STM32F4 Discovery, check your board schematic

CS43L22_STM32 Codec;

#define AUDIOCODEC_CS PD4

void setup()
{
  pinMode(AUDIOCODEC_CS, OUTPUT); // the chip select pin of the codec needs to be enabled

  // digitalWrite(AUDIOCODEC_CS, HIGH);
  // start I2S
  // I2S will also produce MCK for the codec of aroung 12Mhz
  // without MCK the CS43L22 is not working
  I2S.begin(I2S_PHILIPS_MODE, SAMPLINGFREQUENCY, 16);

  digitalWrite(AUDIOCODEC_CS, HIGH); // enable codec
  Codec.begin();
}

#define DACZEROAMPLITUDE 0x4000
int Amplitude = 100;

// speedFactor=1 : normal speed
void playWaveTable(const uint8_t *table, uint32_t tableLength, float duration_sec,  float speedFactor)
{
  uint32_t duration_ms = duration_sec * 1000;
  float index = 0;

  uint32_t startTime = millis();
  while (millis() - startTime < duration_ms)
  {
    if (index < tableLength)
    {
      int dacValue = DACZEROAMPLITUDE + ((int)table[(int)index] - 128) * Amplitude;
      I2S.write(dacValue); // left channel
      I2S.write(dacValue); // right channel 
      index+=speedFactor;
    } else
    {
      I2S.write(DACZEROAMPLITUDE);
      I2S.write(DACZEROAMPLITUDE);
    }
  }
}


void loop()
{
  playWaveTable(piano , SONGLEN, 1, 1);
  playWaveTable(sabine, SONGLEN, 1, 1);
  playWaveTable(piano , SONGLEN, 1, 0.8);
  playWaveTable(sabine, SONGLEN, 1, 1.2);

  float duration=0.5; // start duration
  for(float n=0;n<80;n++)
  {
    float randomSpeedFactor=(float)random(80,200)/100; // for random pitch

    duration = duration * 0.95; // make it faster

    playWaveTable(piano , SONGLEN, duration, randomSpeedFactor);
    playWaveTable(sabine, SONGLEN, duration, randomSpeedFactor);    
  }

  digitalWrite(AUDIOCODEC_CS, LOW); // disable codec,  enough noise, stop it !!
  while(1); // stop
  
  //if (digitalRead( USER_BTN ) ) while (1); // sound off when button pressed

}












