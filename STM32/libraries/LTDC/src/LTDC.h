/*
  Copyright (c) 2017 Daniel Fekete

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

/**
 * LCD-TFT display controller for STM32 F4x9 and F7x[6|7|8|9].
 * Please see AN4861 for more details
 */

#ifndef LTDC_H
#define LTDC_H

#include "Arduino.h"

#include "Adafruit_GFX.h"

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

struct LTDCSettings {
    uint16_t width;
    uint16_t height;
    uint16_t horizontalSync;
    uint16_t horizontalFrontPorch;
    uint16_t horizontalBackPorch;
    uint16_t verticalSync;
    uint16_t verticalFrontPorch;
    uint16_t verticalBackPorch;
};

template <int W, int H>
class LTDCClass : public Adafruit_GFX {
    public:
        LTDCClass(LTDCSettings settings): Adafruit_GFX(W, H){
            this->settings = settings;
        };
        virtual void init() = 0;

        void begin(); // uses buffer from heap
        void begin(uint16_t *buffer);

        void drawPixel(int16_t x, int16_t y, uint16_t color);

        void startWrite(void);
        void writePixel(int16_t x, int16_t y, uint16_t color);
        void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
        void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
        void endWrite(void);

        void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
        void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);


        void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        
        uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
            return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
        }

        uint16_t *buffer = NULL;
        LTDCSettings settings;
        LTDC_HandleTypeDef  hLtdcHandler;

};

template <int W, int H>
void LTDCClass<W, H>::begin() {
    void *buffer = malloc(W * H * sizeof(uint16_t));
    begin((uint16_t *)buffer);
}

template <int W, int H>
void LTDCClass<W, H>::begin(uint16_t *buffer) {
    this->buffer = buffer;

    hLtdcHandler.Init.HorizontalSync = (settings.horizontalSync - 1);
    hLtdcHandler.Init.VerticalSync = (settings.verticalSync - 1);
    hLtdcHandler.Init.AccumulatedHBP = (settings.horizontalSync + settings.horizontalBackPorch - 1);
    hLtdcHandler.Init.AccumulatedVBP = (settings.verticalSync + settings.verticalBackPorch - 1);
    hLtdcHandler.Init.AccumulatedActiveH = (settings.height + settings.verticalSync + settings.verticalBackPorch - 1);
    hLtdcHandler.Init.AccumulatedActiveW = (settings.width + settings.horizontalSync + settings.horizontalBackPorch - 1);
    hLtdcHandler.Init.TotalHeigh = (settings.height + settings.verticalSync + settings.verticalBackPorch + settings.verticalFrontPorch - 1);
    hLtdcHandler.Init.TotalWidth = (settings.width + settings.horizontalSync + settings.horizontalBackPorch + settings.horizontalFrontPorch - 1);

    /* LCD clock configuration */

    /* Initialize the LCD pixel width and pixel height */
    hLtdcHandler.LayerCfg->ImageWidth  = W;
    hLtdcHandler.LayerCfg->ImageHeight = H;

    /* Background value */
    hLtdcHandler.Init.Backcolor.Blue = 0;
    hLtdcHandler.Init.Backcolor.Green = 0;
    hLtdcHandler.Init.Backcolor.Red = 0;

    /* Polarity */
    hLtdcHandler.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    hLtdcHandler.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    hLtdcHandler.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    hLtdcHandler.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
    hLtdcHandler.Instance = LTDC;

    __HAL_RCC_LTDC_CLK_ENABLE();

    init();

    HAL_LTDC_Init(&hLtdcHandler);

    LTDC_LayerCfgTypeDef  layer_cfg;

    /* Layer Init */
    layer_cfg.WindowX0 = 0;
    layer_cfg.WindowX1 = W;
    layer_cfg.WindowY0 = 0;
    layer_cfg.WindowY1 = H;
    layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
    layer_cfg.FBStartAdress = (uint32_t)buffer;
    layer_cfg.Alpha = 255;
    layer_cfg.Alpha0 = 0;
    layer_cfg.Backcolor.Blue = 0;
    layer_cfg.Backcolor.Green = 0;
    layer_cfg.Backcolor.Red = 0;
    layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    layer_cfg.ImageWidth = W;
    layer_cfg.ImageHeight = H;

    HAL_LTDC_ConfigLayer(&hLtdcHandler, &layer_cfg, 0);
}


template <int W, int H>
void LTDCClass<W, H>::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (x >= 0 && x < W && y >= 0 && y < H) {
        buffer[x + y * W] = color;
    }
}

template <int W, int H>
void LTDCClass<W, H>::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    if (x >= W || y < 0 || y >= H) return;

    for(int i=max(x, 0); i<min(x + w, W); i++) {
        buffer[i + y * W] = color;
    }
}

template <int W, int H>
void LTDCClass<W, H>::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    if (x < 0 || x >= W || y >= H) return;

    for(int i=max(y, 0); i<min(y + h, H); i++) {
        buffer[x + i * W] = color;
    }
}


template <int W, int H>
void LTDCClass<W, H>::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (x >= W || y >= H) return;

    for(int j=max(y, 0); j<min(y + h, H); j++) {
        for(int i=max(x, 0); i<min(x + w, W); i++) {
            buffer[i + j * W] = color;
        }
    }
}

template <int W, int H>
void LTDCClass<W, H>::writePixel(int16_t x, int16_t y, uint16_t color) {
    drawPixel(x, y, color);
}

template <int W, int H>
void LTDCClass<W, H>::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    drawFastHLine(x, y, w, color);
}

template <int W, int H>
void LTDCClass<W, H>::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    drawFastVLine(x, y, h, color);
}

template <int W, int H>
void LTDCClass<W, H>::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    fillRect(x, y, w, h, color);
}


template <int W, int H>
void LTDCClass<W, H>::startWrite() {

}

template <int W, int H>
void LTDCClass<W, H>::endWrite() {

}

#endif
