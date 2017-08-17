/*
   Touch example for STM32F476 Discvery

   June 2017, ChrisMicro

*/

#include <Wire.h>
#include "stm32_ub_touch_480x272.h"
#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

uint8_t UB_I2C3_ReadByte(uint8_t addressI2cDevice, uint8_t registerId)
{
  uint8_t result;
  addressI2cDevice = addressI2cDevice >> 1;

  Wire.beginTransmission( addressI2cDevice  );
  Wire.write( registerId );
  uint8_t error;
  error = Wire.endTransmission();

  Wire.requestFrom( addressI2cDevice, (uint8_t) 1 , (uint8_t) true );

  while ( Wire.available() < 1 );

  result = Wire.read() ;

  if (error)Serial.println("I2C error");

  return result;
}

void setup()
{
  Wire.stm32SetInstance(I2C3);
  Wire.stm32SetSDA(PH8);
  Wire.stm32SetSCL(PH7);

  Wire.begin();

  UB_Touch_Init();

  // The buffer is memory mapped
  // You can directly draw on the display by writing to the buffer
  uint16_t *buffer = (uint16_t *)malloc(2*LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height);

  tft.begin((uint16_t *)buffer);
  
  tft.fillScreen(LTDC_BLACK);
  //tft.setRotation(0);
  tft.setCursor(0, 0);
  tft.setTextColor(LTDC_BLUE);  tft.setTextSize(3);
  tft.println("STM32F746 Discovery Touch");

}

#define CURSOR_SIZE 100

int Old_x, Old_y;

void loop()
{
    UB_Touch_Read();
    
    int x = Touch_Data.xp;
    int y = Touch_Data.yp;

    if (Old_x != x || Old_y != y)
    {
      Old_x = x;
      Old_y = y;
      
      tft.fillScreen( LTDC_BLACK );

      tft.setCursor(0, 0);

      tft.print(x); tft.print(" , "); tft.print(y);
      tft.print(" contacts: "); tft.println( P_Touch_GetContacts() );

      if ( P_Touch_GetContacts() )
      {
        tft.fillRect(x - CURSOR_SIZE/2, y - CURSOR_SIZE/2, CURSOR_SIZE, CURSOR_SIZE, LTDC_GREEN);
      }

      delay(10);
    }
}

