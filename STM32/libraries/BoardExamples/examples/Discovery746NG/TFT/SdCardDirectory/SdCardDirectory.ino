/************************************************************************

  STM32F746 Discovery show SD-card directory on TFT

  Required libraries: GFX, Adafruit 

  Installation Instructions:
  1. Import the Adadruit library in the menue
  Sketch=>Include Libraries=>Manage Libraries =>  Adafruit ILI9341   
  2. Import the Adadruit GFX-Library
  Sketch=>Include Libraries=>Manage Libraries =>  Adafruit GFX Library  

  This code is mainly derived fron the "OpenNext" example from the
  SdFat examples.

  June 2017, ChrisMicro

************************************************************************/

#include "SdFat.h"
#include "LTDC_F746_Discovery.h"

SdFatSdio sd;
SdFile file;
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
  tft.setTextColor(LTDC_GREEN);  tft.setTextSize(2);
  tft.println("STM32F746 Discovery SD-card directory");
  tft.setTextColor(LTDC_YELLOW); tft.setTextSize(1);
  
  if (!sd.begin()) 
  {
    tft.setTextColor(LTDC_RED); tft.print("error SD-card not detected");
    sd.initErrorHalt();
  }

  // Open next file in root.  The volume working directory, vwd, is root.
  // Warning, openNext starts at the current position of sd.vwd() so a
  // rewind may be neccessary in your application.
  sd.vwd()->rewind();
  while (file.openNext(sd.vwd(), O_READ)) 
  {
    file.printFileSize(&tft);
    tft.write(' ');
    file.printModifyDateTime(&tft);
    tft.write(' ');
    file.printName(&tft);
    if (file.isDir()) 
    {
      // Indicate a directory.
      tft.write('/');
    }
    tft.println();
    delay(1000);
    file.close();
  }
  tft.setTextColor(LTDC_BLUE);
  tft.println("Done!");
}

void loop()
{
}
