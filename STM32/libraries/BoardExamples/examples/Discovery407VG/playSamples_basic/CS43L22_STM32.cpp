/*

 CS43L22 codec class for Arduino STM32GENERIC

 April 2017, ChrisMicro
 
 */

// The CS43L22 is a Low Power, Stereo DAC w/Headphone & Speaker Amps
//
// related links:
// data sheet CS43L22: https://d3uzseaevmutz1.cloudfront.net/pubs/proDatasheet/CS43L22_F2.pdf
// https://github.com/abhishek-kakkar/STM32F4Player/blob/master/STM32F4_Discovery_demo/codec_CS43L22.c
// https://www.mikrocontroller.net/topic/413574#4814689

#include "CS43L22_STM32.h"

CS43L22_STM32::CS43L22_STM32()
{
  // before calling this class
  // remember for correct working codec:
  // 1. enable code clock ( usually done by I2S driver )
  // enable codec chip select line 
}

#define CODEC_ADDR 0x4A // i2c address ( 2*0x4A=0x94)

void codec_writeReg(unsigned char reg, unsigned char data)
{
  uint8_t error;
  Wire.beginTransmission(CODEC_ADDR);
  Wire.write(reg);
  Wire.write(data);
  error = Wire.endTransmission();
}

// codec setup from
// https://github.com/mubase/STM32F4-Arduino-core/blob/master/cores/maple/libmaple/stm32f4codec/codec.h
// TBD: to be refactured
void codec_reg_setup(void)
{
  Wire.begin();

  codec_writeReg(0x02, 0x01); // power save registers -> all on
  codec_writeReg(0x00, 0x99);
  codec_writeReg(0x47, 0x80); //inits
  codec_writeReg(0xd, 0x3); // playback ctrl
  codec_writeReg(0x32, (1 << 7)); // vol
  codec_writeReg(0x32, (0 << 7)); // vol
  codec_writeReg(0x00, 0x00); // inits
  codec_writeReg(0x04, 0xaf); // power ctrl
  codec_writeReg(0x0d, 0x70);
  codec_writeReg(0x05, 0x81); // clocking
  codec_writeReg(0x06, 0x07);
  codec_writeReg(0x0a, 0x00);
  codec_writeReg(0x27, 0x00);
  codec_writeReg(0x80, 0x0a); // both channels on
  codec_writeReg(0x1f, 0x0f);
  codec_writeReg(0x02, 0x9e);

}

void CS43L22_STM32::begin()
{
  Wire.begin();
  codec_reg_setup();
  
}

