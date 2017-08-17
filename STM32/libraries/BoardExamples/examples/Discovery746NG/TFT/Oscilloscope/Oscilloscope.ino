/************************************************************************

  STM32F546 Discovery simple oscilloscope example

  The oscilloscope has a auto trigger function to achieve a stable
  visual impression when a repeating signal like a sine wave is applied.

  May 2017, ChrisMicro

************************************************************************/

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

#define HORIZONTAL_RESOLUTION 480
#define VERTICAL_RESOLUTION   250
#define POSITION_OFFSET_Y      20
#define SIGNAL_LENGTH HORIZONTAL_RESOLUTION

uint16_t oldSignal[SIGNAL_LENGTH];
uint16_t adcBuffer[SIGNAL_LENGTH];

#define SAMPLING_TIME_US      5
#define ANALOG_SIGNAL_INPUT PA0

void setup()
{
  // The buffer is memory mapped
  // You can directly draw on the display by writing to the buffer
  uint16_t *buffer = (uint16_t *)malloc(2*LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height);

  tft.begin((uint16_t *)buffer);
  tft.fillScreen( LTDC_BLACK );
  tft.fillRect(10, 1, 150, 160, LTDC_BLACK);
  tft.setRotation(1); // horitzontal wide screen
  tft.setCursor(0, 0);
  tft.setTextColor(LTDC_GREEN);  //tft.setTextSize(3);
  tft.setTextSize(1);

  tft.println("analog input PA0");
  tft.print("sampling frequency: "); tft.print(1000000/SAMPLING_TIME_US); tft.println(" Hz");
}

void waitForAutoTrigger()
{

  uint32_t triggerLevel = 512;
  uint32_t hysteresis   =  10;
  uint32_t timeOut_ms   = 100;

  uint32_t timeOutLimit = millis() + timeOut_ms;
  uint32_t timeOutFlag = false;

  uint32_t adcValue=0;

  adcValue = analogRead( ANALOG_SIGNAL_INPUT );

  // wait for low level
  while ( ( adcValue > triggerLevel - hysteresis ) && !timeOutFlag )
  {
    adcValue = analogRead( ANALOG_SIGNAL_INPUT );
    
    if ( millis() > timeOutLimit ) timeOutFlag = 1 ;
  }

  if ( !timeOutFlag )
  {
    // wait for high level
    while ( ( adcValue < triggerLevel + hysteresis ) && ( millis() < timeOutLimit ) )
    {
      adcValue = analogRead(ANALOG_SIGNAL_INPUT);
    }
  }

}

void showSignal()
{
  int n;

  int oldx;
  int oldy;
  int y;

  for (n = 1; n < SIGNAL_LENGTH; n++)
  {
    y = VERTICAL_RESOLUTION - ( adcBuffer[n] * VERTICAL_RESOLUTION / 1024 ) + POSITION_OFFSET_Y;

    if (n == 1)
    {
      oldx = n;
      oldy = y;
    }

    if (n < SIGNAL_LENGTH - 1)
    {
      // delete old line element
      tft.drawLine(n - 1 , oldSignal[n - 1], n, oldSignal[n], LTDC_BLACK );

      // draw new line element
      tft.drawLine(oldx,               oldy, n,            y, LTDC_GREEN );
    }
    
    oldSignal[n - 1] = oldy;
    oldx = n;
    oldy = y;
  }
}

void loop(void)
{
  int n;
  uint32_t nextTime=0;
  
  waitForAutoTrigger();

  // record signal
  for (n = 1; n < SIGNAL_LENGTH; n++)
  {
    adcBuffer[n] = analogRead( ANALOG_SIGNAL_INPUT );

    // wait for next sample
    while(micros()<nextTime); 
    nextTime=micros()+SAMPLING_TIME_US;   
  }

  showSignal();
}


