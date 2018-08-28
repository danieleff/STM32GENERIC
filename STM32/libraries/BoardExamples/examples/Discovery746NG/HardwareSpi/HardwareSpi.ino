/************************************************************************

  STM32F746 Discovery Hardware SPI example 
  
  June 2017, ChrisMicro

************************************************************************/
#include "SPI.h"

// SPI pins at the STM32F746 discovery Arduino Pin Header
#define SPI_CS                 PA8 //D10
#define SPI_MOSI               PB15 //D11
#define SPI_MISO               PB14 //D12
#define SPI_SCK                PI1 //D13

void setup() 
{
  pinMode(SPI_CS, OUTPUT);
  
  SPI.stm32SetInstance(SPI2);
  SPI.stm32SetMOSI(SPI_MOSI);
  SPI.stm32SetMISO(SPI_MISO);
  SPI.stm32SetSCK(SPI_SCK);
  
  SPI.begin();
}

uint32_t Counter=0;

void loop() 
{
  digitalWrite(SPI_CS,LOW);
  SPI.beginTransaction(SPISettings(16000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(Counter++);
  SPI.endTransaction();
  digitalWrite(SPI_CS,HIGH);
  delay(10);
}
