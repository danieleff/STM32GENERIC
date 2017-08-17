/**************************************************************************

  STM32F746 mandelbrot calculation

  Addapted from a mix of different sources to display the mandelbrot set.
  
  June 2017, ChrisMicro

**************************************************************************/

#include "LTDC_F746_Discovery.h"

LTDC_F746_Discovery tft;

/*
Adafruit NeoPixelColorWheel
RGB_Color=Wheel(colorNumber)
converts a colorNumber on a color wheel from 0 to 255 into a RGB color.
https://color.adobe.com/de/create/color-wheel/?base=2&rule=Analogous
*/
uint16_t colorWheel( uint16_t WheelPos ) 
{
  uint32_t r,g,b;
  WheelPos = 255 - WheelPos;
  
  if (WheelPos < 85) {
    r = 255 - WheelPos * 3;
    g = 0;
    b = WheelPos * 3;
  }else
  if (WheelPos < 170) {
    WheelPos -= 85;
    r = 0;
    g = WheelPos * 3;
    b = 255 - WheelPos * 3;
  }else
  {
    WheelPos -= 170;
    r = WheelPos * 3;
    g = 255 - WheelPos * 3;
    b = 0;
  }
  uint16_t LTDC_color=tft.color565(r,g,b);
  
  return LTDC_color;
}

void setup()
{
  // The buffer is memory mapped
  // You can directly draw on the display by writing to the buffer
  uint16_t *buffer = (uint16_t *)malloc(2*LTDC_F746_ROKOTECH.width * LTDC_F746_ROKOTECH.height);

  tft.begin((uint16_t *)buffer);
  tft.fillScreen( LTDC_BLACK );

  //tft.setRotation(0); // horitzontal wide screen

  for(int n=0;n<255;n++) tft.drawPixel( n, 1 , colorWheel(n) );
  delay(3000);
}

// mandelbrot code from
// https://rosettacode.org/wiki/Category:C
// modified and reduced by ChrisMicro

void loop()
{
  /* screen ( integer) coordinate */
  int iX, iY;
  const int iXmax = 480;
  const int iYmax = 272;
  /* world ( float) coordinate = parameter plane*/
  float Cx, Cy;
  const float factor=0.8;
  const float CxMin = -2.8*factor;
  const float CxMax = 2.0*factor;
  const float CyMin = -1.5*factor;
  const float CyMax = 1.5*factor;
  /* */
  float PixelWidth = (CxMax - CxMin) / iXmax;
  float PixelHeight = (CyMax - CyMin) / iYmax;

  /* Z=Zx+Zy*i  ;   Z0 = 0 */
  float Zx, Zy;
  float Zx2, Zy2; /* Zx2=Zx*Zx;  Zy2=Zy*Zy  */

  int Iteration;
  const int IterationMax = 220;
  /* bail-out value , radius of circle ;  */
  const float EscapeRadius = 2;
  float ER2 = EscapeRadius * EscapeRadius;

  /* compute and write image data bytes */
  for (iY = 0; iY < iYmax; iY++)
  {
    Cy = CyMin + iY * PixelHeight;
    if (fabs(Cy) < PixelHeight / 2) Cy = 0.0; /* Main antenna */
    for (iX = 0; iX < iXmax; iX++)
    {
      Cx = CxMin + iX * PixelWidth;
      /* initial value of orbit = critical point Z= 0 */
      Zx = 0.0;
      Zy = 0.0;
      Zx2 = Zx * Zx;
      Zy2 = Zy * Zy;
      
      for (Iteration = 0; Iteration < IterationMax && ((Zx2 + Zy2) < ER2); Iteration++)
      {
        Zy = 2 * Zx * Zy + Cy;
        Zx = Zx2 - Zy2 + Cx;
        Zx2 = Zx * Zx;
        Zy2 = Zy * Zy;
      };

      uint16_t LTDC_color;
      if( Iteration>10) LTDC_color = colorWheel(Iteration+20);
      else LTDC_color = LTDC_BLACK;
      
      tft.drawPixel(iX, iY, LTDC_color);
    }
  }

  delay(3000);
  tft.fillScreen( LTDC_BLACK );
}

