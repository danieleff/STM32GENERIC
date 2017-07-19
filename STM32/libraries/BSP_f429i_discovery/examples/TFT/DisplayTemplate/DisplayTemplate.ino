/************************************************************************

  STM32F429 Discovery TFT example template

  Required libraries: GFX, Adafruit 

  Installation Instructions:
  1. Import the Adadruit library in the menue
  Sketch=>Include Libraries=>Manage Libraries =>  Adafruit ILI9341   
  2. Import the Adadruit GFX-Library
  Sketch=>Include Libraries=>Manage Libraries =>  Adafruit GFX Library  


  June 2017, ChrisMicro

************************************************************************/

#include "TFT_F429_Discovery.h"

TFT_F429_Discovery tft;

void setup()
{
  tft.begin();
  tft.fillScreen(LTDC_BLACK);

  //tft.setRotation(1); 
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


