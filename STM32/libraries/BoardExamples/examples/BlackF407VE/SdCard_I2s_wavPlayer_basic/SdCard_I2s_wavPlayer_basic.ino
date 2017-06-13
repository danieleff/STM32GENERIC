/*
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !! important: read installation description below !!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   basic wav file player example

   - read the file "test.wav" on the sd-card
   - play 1 second of the file

   The time playing is limited by the available RAM of the microcontroller.

   The test.wav file should have the following format:
   16Bit stereo, 44100kHz 

   Hardware:
    microcontroller board: STM32F407VE Black
    amplifier:             Adafruit I2S amplifier with the MAX98357a chip
                          https://www.adafruit.com/product/3006
   
   MAY 2017, ChrisMicro

*/


#include "SdFat.h"
#include "I2S.h"

I2SClass I2S(SPI2, PC3 /*DIN*/ , PB12 /*LRC*/, PB10 /*SCLK*/);

SdFatSdio SD;

File sFile;

#define DAC_DC_VALUE      0x4000
#define DAC_MAX_AMPLITUDE 0x2000 // experimental maximum on my speakers without distortion

#define SAMPLINGFREQUENCY 44100

#define WAVEBUFFERLENGTH  SAMPLINGFREQUENCY * 1 // 1 seconds, time is limited due to RAM capacity

int16_t wave_i16[WAVEBUFFERLENGTH];

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


void beep() // used to signal that the I2S amplifier is working
{
  float   frequency = 2000;
  float   amplitude = DAC_MAX_AMPLITUDE;

  // calculate buffer values for test tone
  for ( int n = 0; n < WAVEBUFFERLENGTH; n++ ) wave_i16[n] = ( sin( 2 * PI * frequency / SAMPLINGFREQUENCY * n ) ) * amplitude;

  digitalWrite( LED_BUILTIN, LOW ); // led on
  playBuffer( wave_i16, WAVEBUFFERLENGTH / 4 );
  digitalWrite( LED_BUILTIN, HIGH ); // led off
}


// wav file format description
// https://de.wikipedia.org/wiki/RIFF_WAVE#Beispiel_eines_allgemein_lesbaren_WAVE-PCM-Formats

void readWavFileStereo16Bit_into_MonoBuffer(char * fileName)
{
  sFile.open(fileName);

  sFile.seekSet(44); // start of data block, see waf RIFF format spec

  for (int n = 0; n < WAVEBUFFERLENGTH; n++)
  {
    int16_t value = 0;

    // left channel
    value = sFile.read();
    value += sFile.read() << 8;
    wave_i16[n] = (int32_t) value * DAC_MAX_AMPLITUDE / 0x8000;

    // right channel has to be read but will not be stored into the mono buffer
    sFile.read();
    sFile.read();
  }

  sFile.close();
}

void setup()
{
  pinMode(USER_BUTTON1, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  I2S.begin(I2S_PHILIPS_MODE, SAMPLINGFREQUENCY, 16);
  beep();

  Serial.begin(115200);

  for (int i = 0; i < 5; i++) {
    delay(1000);
    Serial.print(".");
  }

}

void loop()
{
  while (!SD.begin())
  {
    Serial.println("Card error");
    beep();
    delay(1000);
  }

  readWavFileStereo16Bit_into_MonoBuffer( "test.wav" );
  playBuffer( wave_i16, WAVEBUFFERLENGTH );

  delay(300);
  Serial.println("");
  
  while (digitalRead( USER_BUTTON1 ) != 0) // wait for user action to play again
  {
    // blink while waiting
    digitalWrite( LED_BUILTIN, LOW ); // led on
    delay(200);
    digitalWrite( LED_BUILTIN, HIGH ); // led off
    delay(200);
  }
}



