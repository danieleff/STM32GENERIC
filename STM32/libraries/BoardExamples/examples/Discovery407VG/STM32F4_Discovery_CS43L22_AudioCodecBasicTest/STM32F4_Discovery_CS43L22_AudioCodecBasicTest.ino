/* 
  STM32F4 Discovery basic audio codec setup example

  This example sets up the CS43L22 and generates a sawtooth sound
  at the audio jack output.

  April 2014, ChrisMicro

  Special thanks to Daniel Fekete for his gread STM32GENERIC support  

*/
  
 
// The CS43L22 is a Low Power, Stereo DAC w/Headphone & Speaker Amps
//
// related links:
// data sheet CS43L22: https://d3uzseaevmutz1.cloudfront.net/pubs/proDatasheet/CS43L22_F2.pdf
// https://github.com/abhishek-kakkar/STM32F4Player/blob/master/STM32F4_Discovery_demo/codec_CS43L22.c
// https://www.mikrocontroller.net/topic/413574#4814689

 
#include <Wire.h>
#include "I2S.h"

I2SClass I2S(SPI3, PC12 /*sd*/ , PA4 /*ws*/, PC10 /*ck*/, PC7/* MCK*/); // setup for STM32F4 Discovery, check your board schematic

#define SERIALINTERFACE SerialUSB

#define AUDIOCODEC_CHIP_SELECT PD4

#define CODEC_I2C_ADDR 0x4A // i2c address ( 2*0x4A = 0x94 )

void codec_writeReg(unsigned char reg, unsigned char data)
{
  Wire.beginTransmission(CODEC_I2C_ADDR);
  Wire.write(reg);
  Wire.write(data);
}

enum CodecRegister {
  ID                        = 1,
  POWER_CTL1                = 2,
  POWER_CTL2                = 4,
  CLOCKING_CTL              = 5,
  INTERFACE_CTL1            = 6,
  INTERFACE_CTL2            = 7,
  PASSTHROUGH_A             = 8,
  PASSTHROUGH_B             = 9,
  ANALOG_ZC_SR              = 0xa,
  PASSTHROUGH_GANG_CTL      = 0xc,
  PLAYBACK_CTL1             = 0xd,
  MISC_CTL                  = 0xe,
  PLAYBACK_CTL2             = 0xf,
  PASSTHROUGH_A_VOL         = 0x14,
  PASSTHROUGH_B_VOL         = 0x15,
  PCMA_VOL                  = 0x1a,
  PCMB_VOL                  = 0x1b,
  BEEP_FREQ_ON_TIME         = 0x1c,
  BEEP_VOL_OFF_TIME         = 0x1d,
  BEEP_TONE_CFG             = 0x1e,
  TONE_CTL                  = 0x1f,
  MASTER_A_VOL              = 0x20,
  MASTER_B_VOL              = 0x21,
  HEADPHONE_A_VOL           = 0x22,
  HEADPHONE_B_VOL           = 0x23,
  SPEAKER_A_VOL             = 0x24,
  SPEAKER_B_VOL             = 0x25,
  CHANNEL_MIXER_AND_SWAP    = 0x26,
  LIMIT_CTL1_THRESHOLDS     = 0x27,
  LIMIT_CTL2_RELEASE_RATE   = 0x28,
  LIMITER_ATTACK_RATE       = 0x29,
  OVERFLOW_AND_CLOCK_STATUS = 0x2e,
  BATTERY_COMPENSATION      = 0x2f,
  VP_BATTERY_LEVEL          = 0x30,
  SPEAKER_STATUS            = 0x31,
  CHARGE_PUMP_FREQUENCY     = 0x34
};


// each register retries until success
// if communication fails the device will hang
// codec setup from
// https://github.com/mubase/STM32F4-Arduino-core/blob/master/cores/maple/libmaple/stm32f4codec/codec.h
// TBD: to be simplified and reworked

void codec_reg_setup(void) 
{
  codec_writeReg(0x02, 0x01); // power save registers -> all on
  codec_writeReg(0x00, 0x99);
  codec_writeReg(0x47, 0x80); //inits
  codec_writeReg(0x0d, 0x03); // playback ctrl
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

void setup()
{
  pinMode(USER_BTN,      INPUT); //user button for sound stop, mandatory for STM MCUs if you want to use the button
  pinMode(AUDIOCODEC_CHIP_SELECT, OUTPUT); // the chip select pin of the codec needs to be enabled 
  
  //digitalWrite(AUDIOCODEC_CHIP_SELECT, HIGH);
  // start I2S
  // I2S will also produce MCK for the codec of aroung 12Mhz
  // without MCK the CS43L22 is not working
  I2S.begin(I2S_PHILIPS_MODE, 44000, 16);  

  // MCK is running, enable codec
  digitalWrite(AUDIOCODEC_CHIP_SELECT, HIGH); 

  // start the I2C driver for codec register setup  
  Wire.begin(); 
  codec_reg_setup();

}

void loop()
{
  static uint16_t n;
  I2S.write(n += 300); // produce sound: simple triangle waveform

  if  ( digitalRead( USER_BTN ) ) // stop sound when user button pressed
  {
    while (1) 
    {

    }
  }

}





