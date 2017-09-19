/*
   Touch example for STM32F429 Discvery

   Sept. 2017, ChrisMicro
*/

#include <Wire.h>
#include "stm32_ub_touch_stmpe811.h"
#include "LTDC_F429_Discovery.h"

LTDC_F429_Discovery tft;
TwoWire Wire3(I2C3,PC9,PA8);

uint8_t UB_I2C3_ReadByte(uint8_t addressI2cDevice, uint8_t registerId)
{
  uint8_t result;
  addressI2cDevice = addressI2cDevice >> 1;

  Wire3.beginTransmission( addressI2cDevice  );
  Wire3.write( registerId );
  uint8_t error;
  error = Wire3.endTransmission();

  Wire3.requestFrom( addressI2cDevice, (uint8_t) 1 , (uint8_t) true );

  while ( Wire3.available() < 1 );

  result = Wire3.read() ;
  return result;
}

int16_t UB_I2C3_WriteByte(uint8_t addressI2cDevice, uint8_t adr, uint8_t wert)
{
  uint16_t error=0;
  
  addressI2cDevice = addressI2cDevice >> 1;

  Wire3.beginTransmission( addressI2cDevice  );
  Wire3.write( adr );
  Wire3.write( wert );
  error = Wire3.endTransmission();

  return error;
}

void setup()
{
  Wire3.begin();

  tft.begin();
  
  tft.fillScreen(LTDC_BLACK);
  //tft.setRotation(0);
  tft.setCursor(0, 0);
  tft.setTextColor(LTDC_BLUE);  tft.setTextSize(3);
  tft.println("STM32F429 Discovery Touch");
  Serial.println("step 1");
  
  if(UB_Touch_Init()==ERROR)
  {
    tft.println("touch interface error");
    tft.println("stopped!");
    while(1);
  }
}

#define CURSOR_SIZE 60

int Old_x, Old_y;

void loop()
{
    UB_Touch_Read();
    
    int x = Touch_Data.xp;
    int y = Touch_Data.yp;
    int touchState=Touch_Data.status;

    if ( touchState ==TOUCH_PRESSED )    
    {
      if(Old_x != x || Old_y != y) // reduce flickering
      {
        Old_x = x;
        Old_y = y;
        
        tft.fillScreen( LTDC_BLACK );
        tft.setCursor(0, 0);
        tft.print(x); tft.print(" , "); tft.print(y);
        tft.fillRect(x - CURSOR_SIZE/2, y - CURSOR_SIZE/2, CURSOR_SIZE, CURSOR_SIZE, LTDC_GREEN);
      }
      delay(10); 
    }
}

