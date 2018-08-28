/*

 I2S sine wave for STM32 DAC
 
 PA4_DACOUT and PA5_DACOUT are predefined instances of class STM32DAC.
 
 August 2018 ChrisMicro

 uses the DAC driver from huaweiwx 
*/

#include "STM32DAC.h"
#define SAMPLINGFREQUENCY 44100
#define NUMBEROFSAMPLES  SAMPLINGFREQUENCY * 1 // 1 seconds

#define DAC_RESOLUTION 4095 // DAC resoltion is 12 bit
#define DAC_MAX_AMPLITUDE DAC_RESOLUTION/2/2; 

int16_t Buffer[NUMBEROFSAMPLES];

void setup()
{  
  PA4_DACOUT.Init();
  
  for (int n = 0; n < NUMBEROFSAMPLES; n++) 
  {
    const float   frequency = 440;
    const float   amplitude = DAC_MAX_AMPLITUDE;
    Buffer[n] = DAC_RESOLUTION/2+( sin( 2 * PI * frequency / SAMPLINGFREQUENCY * n )) * amplitude;
  }
}

void loop()
{
  for (int n = 0; n < NUMBEROFSAMPLES; n++) 
  {
    delayMicroseconds(1000000/SAMPLINGFREQUENCY-1);
    PA4_DACOUT = Buffer[n];
  }
  delay(3000);
}
