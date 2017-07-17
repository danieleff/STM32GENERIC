/*
   Wrapper for STM LTDC to Adafruit GFX driver

   What is LTDC?
   http://www.st.com/content/ccc/resource/technical/document/application_note/group0/25/ca/f9/b4/ae/fc/4e/1e/DM00287603/files/DM00287603.pdf/jcr:content/translations/en.DM00287603.pdf

   2017,ChrisMicro


   To be done: replase this by a faster direct
   display driver.

*/
#ifndef _TFT_F429_DISCOVERY_
#define _TFT_F429_DISCOVERY_

#include "Arduino.h"

#include "Adafruit_GFX.h"
//#include "BSP_f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"

#define LTDC_BLACK       0x0000      /*   0,   0,   0 */
#define LTDC_NAVY        0x000F      /*   0,   0, 128 */
#define LTDC_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define LTDC_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define LTDC_MAROON      0x7800      /* 128,   0,   0 */
#define LTDC_PURPLE      0x780F      /* 128,   0, 128 */
#define LTDC_OLIVE       0x7BE0      /* 128, 128,   0 */
#define LTDC_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define LTDC_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define LTDC_BLUE        0x001F      /*   0,   0, 255 */
#define LTDC_GREEN       0x07E0      /*   0, 255,   0 */
#define LTDC_CYAN        0x07FF      /*   0, 255, 255 */
#define LTDC_RED         0xF800      /* 255,   0,   0 */
#define LTDC_MAGENTA     0xF81F      /* 255,   0, 255 */
#define LTDC_YELLOW      0xFFE0      /* 255, 255,   0 */
#define LTDC_WHITE       0xFFFF      /* 255, 255, 255 */
#define LTDC_ORANGE      0xFD20      /* 255, 165,   0 */
#define LTDC_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define LTDC_PINK        0xF81F


class TFT_F429_Discovery : public Adafruit_GFX {
  public:
    TFT_F429_Discovery(): Adafruit_GFX((int16_t) 240, (int16_t) 320)
    {

    }

    void init()
    {

    };

    void begin()
    {
      BSP_LCD_Init();
      BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER);
      BSP_LCD_SelectLayer(1);
      BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

      BSP_LCD_SetTransparency(1, 255);

      /* Clear the LCD */
      BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
      BSP_LCD_Clear(LCD_COLOR_WHITE);

    };

    uint32_t RGB565TORGB8888(int16_t RGB565)
    {
      int color = RGB565;
      uint8_t r = ((color >> 11) & 0x1F) << 3;
      uint8_t g = ((color >> 5) & 0x3F) << 2;
      uint8_t b = (color & 0x1F) << 3;
      uint32_t RGB8888 = 0XFF000000 | r << 16 | g << 8 | b;

      return RGB8888;
    }

    void setColor(int16_t RGB565)
    {
      BSP_LCD_SetTextColor(RGB565TORGB8888(RGB565));
    }

    void drawPixel(int16_t x, int16_t y, uint16_t color)
    {
      BSP_LCD_DrawPixel(x, y, RGB565TORGB8888(color));
    };

    void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
    {
      setColor(color);
      BSP_LCD_DrawHLine(x, y, w);
    };

    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
    {
      setColor(color);
      BSP_LCD_DrawVLine(x, y, h);
    };
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
    {
      setColor(color);
      BSP_LCD_DrawHLine(x, y, w);
    };

    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
    {

      //BSP_LCD_SetColorKeying(0, color); // keine Farbänderung

      setColor(color);
      BSP_LCD_FillRect(x, y, w, h); // Rechteck bleibt schwarz
    };

    uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
      return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
    }

};
#endif // TFT_F429_DISCOVERY
