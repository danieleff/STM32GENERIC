/************************************************************************

  STM32F746 Discovery Voltmeter 

  Read the voltage on the ADC and show it on the TFT display.

  Be careful:

  ! Do not attache more than 3.3V to the analog input !

  June 2017, ChrisMicro

************************************************************************/

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

#define ANALOG_SIGNAL_INPUT PA0

void setup()
{
  // The buffer is memory mapped
  // You can directly draw on the display by writing to the buffer
  uint16_t *buffer = (uint16_t *)malloc(2*LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height);

  tft.begin((uint16_t *)buffer);
  tft.fillScreen( LTDC_BLACK );

  //tft.setRotation(0); // horitzontal wide screen
}

#define SAMPLING_TIME_MS 1000

void loop(void)
{
  float    adcValue = 0;
  uint32_t counts   = 0;
  uint32_t endTime;
  
  // meanvalue over a period 
  endTime= millis() + SAMPLING_TIME_MS;
  while(millis()<endTime)
  {
    adcValue += analogRead( ANALOG_SIGNAL_INPUT );
    counts++;
  } 
  adcValue = adcValue / counts;

  tft.fillScreen(LTDC_BLACK);
  
  tft.setCursor(0, 0);  
  tft.setTextColor(LTDC_RED);
  tft.setTextSize(1);
  tft.println("analog input PA0");

  tft.setTextColor(LTDC_GREEN);
  tft.setTextSize(3);
  tft.print("adc value:"); tft.println( (int) adcValue);

  tft.setCursor(150, 110);
  tft.setTextColor(LTDC_YELLOW); 
  tft.setTextSize(5);
  float voltage = adcValue / 1024 * 3.3;
  tft.print( voltage, 3 ); // print 3 post decimal positions
  tft.print(" V");
}

