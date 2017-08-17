/************************************************************************

  STM32F746 Discovery TFT example template

  Required libraries: GFX, Adafruit 

  Installation Instructions:
  1. Import the Adadruit library in the menue
  Sketch=>Include Libraries=>Manage Libraries =>  Adafruit ILI9341   
  2. Import the Adadruit GFX-Library
  Sketch=>Include Libraries=>Manage Libraries =>  Adafruit GFX Library  


  June 2017, ChrisMicro

************************************************************************/

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

void setup()
{
  // The buffer is memory mapped
  // You can directly draw on the display by writing to the buffer
  uint16_t *buffer = (uint16_t *)malloc(2*LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height);

  tft.begin((uint16_t *)buffer);
  tft.fillScreen(LTDC_BLACK);
  //tft.setRotation(0); 
  tft.setCursor(0, 0);
  tft.setTextColor(LTDC_GREEN);  tft.setTextSize(3);
  tft.println("STM32F746 Discovery");
  tft.setTextColor(LTDC_YELLOW); tft.setTextSize(2);
}

int Counter=0;

void loop(void)
{
  tft.print( Counter ); tft.print(" ");
  Counter ++;
  delay(1000);
}


