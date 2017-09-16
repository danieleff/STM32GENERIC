/************************************************************************

  STM32F429 Discovery Voltmeter 

  Read the voltage on the ADC and show it on the TFT display.

  Be careful:

  ! Do not attache more than 3.3V to the analog input !

  Sept. 2017, ChrisMicro

  Remark: at the moment only the ADC connected to PA0 seems to return
  useful values. PA0 is connected to the blue button on the board.

************************************************************************/

#include "LTDC_F429_Discovery.h"

LTDC_F429_Discovery tft;

#define ANALOG_SIGNAL_INPUT PA0

void setup()
{
  tft.begin();
  tft.fillScreen( LTDC_BLACK );
 
  //tft.setRotation(0); // horitzontal wide screen
}

#define SAMPLING_TIME_MS 1000

void loop(void)
{
  float    adcValue = 0;
  uint32_t counts   = 0;
  uint32_t endTime;
  
  // filter the signal
  // mean value over a period 
  endTime= millis() + SAMPLING_TIME_MS;
  while(millis()<endTime)
  {
    adcValue += analogRead( ANALOG_SIGNAL_INPUT );
    counts++;
  } 
  adcValue = adcValue / counts;

  tft.fillScreen(LTDC_BLACK);
  
  tft.setCursor(0, 0);  
  tft.setTextColor(LTDC_BLUE);
  tft.setTextSize(2);
  tft.println("analog input PA0");
  tft.println("(blue button)");
  
  tft.setTextColor(LTDC_GREEN);
  tft.setTextSize(3);
  tft.print("adc val:"); tft.println( (int) adcValue);

  tft.setCursor(20, 150);
  tft.setTextColor(LTDC_YELLOW); 
  tft.setTextSize(5);
  float voltage = adcValue / 1024 * 3.3;
  tft.print( voltage, 3 ); // print 3 post decimal positions
  tft.print(" V");
}

