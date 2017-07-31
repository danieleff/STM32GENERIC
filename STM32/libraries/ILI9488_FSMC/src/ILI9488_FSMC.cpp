#include "ILI9488_FSMC.h"
#include <Arduino.h>

ILI9488_FSMC::ILI9488_FSMC(void) : Adafruit_GFX(ILI9488_TFTWIDTH, ILI9488_TFTHEIGHT) {
}

void ILI9488_FSMC::begin(void) {

  // TODO - Still need to add reset methods
  
  // Lets just to a hard reset on the LCD module for the heck of it.
//  RS_H;
  delay(5);
//  RS_L;
  delay(20);
//  RS_H;
  delay(150);

  writecommand(0x11);
  delay(20);  

  writecommand(0xE0);
   writedata(0x00);
   writedata(0x03);
   writedata(0x09);
   writedata(0x08);
   writedata(0x16);
   writedata(0x0A);
   writedata(0x3F);
   writedata(0x78);
   writedata(0x4C);
   writedata(0x09);
   writedata(0x0A);
   writedata(0x08);
   writedata(0x16);
   writedata(0x1A);
   writedata(0x0F);
   
  writecommand(0XE1);
   writedata(0x00);
   writedata(0x16);
   writedata(0x19);
   writedata(0x03);
   writedata(0x0F);
   writedata(0x05);
   writedata(0x32);
   writedata(0x45);
   writedata(0x46);
   writedata(0x04);
   writedata(0x0E);
   writedata(0x0D);
   writedata(0x35);
   writedata(0x37);
   writedata(0x0F);
   
  writecommand(0XC0);      			//Power Control 1
   writedata(0x17);    				//Vreg1out
   writedata(0x15);    				//Verg2out
  writecommand(0xC1);      			//Power Control 2
   writedata(0x41);    				//VGH,VGL
  writecommand(0xC5);      			//Power Control 3
   writedata(0x00);
   writedata(0x12);    				//Vcom
   writedata(0x80);
  writecommand(0x36);      			//Memory Access
   writedata(0x48);
  writecommand(0x3A);      			// Interface Pixel Format
   writedata(0x55); 	  			// 16bit
  writecommand(0XB0);      			// Interface Mode Control
   writedata(0x00);     			//SDO NOT USE	// was 0x80 on SPI mode
  writecommand(0xB1);      			//Frame rate
   writedata(0xA0);    				//60Hz
  writecommand(0xB4);      			//Display Inversion Control
   writedata(0x02);    				//2-dot
  writecommand(0XB6);      			//Display Function Control  RGB/MCU Interface Control
  writedata(0x02);    				//MCU
   writedata(0x02);    				//Source,Gate scan dieection
  writecommand(0XE9);      			// Set Image Functio
   writedata(0x00);    				// Disable 24 bit data
  writecommand(0xF7);      			// Adjust Control
   writedata(0xA9);
   writedata(0x51);
   writedata(0x2C);
   writedata(0x82);    				// D7 stream, loose
  writecommand(ILI9488_SLPOUT);    	//Exit Sleep
  delay(120);
  writecommand(ILI9488_DISPON);    	//Display on */
}

void ILI9488_FSMC::bl_on(void) {
//  BL_H;
}

void ILI9488_FSMC::bl_off(void) {
//  BL_L;
}

void ILI9488_FSMC::writecommand(uint8_t c) {
  ILI9488_REG=c;
}

void ILI9488_FSMC::writedata(uint8_t d) {
  ILI9488_RAM=d;
}

void ILI9488_FSMC::setScrollArea(uint16_t topFixedArea, uint16_t bottomFixedArea) {
  writecommand(0x33);
   writedata(topFixedArea >> 8);
   writedata(topFixedArea);
   writedata((_height - topFixedArea - bottomFixedArea) >> 8);
   writedata(_height - topFixedArea - bottomFixedArea);
   writedata(bottomFixedArea >> 8);
   writedata(bottomFixedArea);
}

void ILI9488_FSMC::scroll(uint16_t pixels) {
  writecommand(0x37); 
   writedata(pixels >> 8);
   writedata(pixels);
}

void ILI9488_FSMC::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  writecommand(ILI9488_CASET); 	// Column addr set
   writedata(x0 >> 8);
   writedata(x0 & 0xFF);     	// XSTART
   writedata(x1 >> 8);
   writedata(x1 & 0xFF);     	// XEND
  writecommand(ILI9488_PASET); 	// Row addr set
   writedata(y0>>8);
   writedata(y0 &0xff);     	// YSTART
   writedata(y1>>8);
   writedata(y1 &0xff);     	// YEND
  writecommand(ILI9488_RAMWR); 	// write to RAM
}

void ILI9488_FSMC::startAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  setAddrWindow(x0, y0, x1, y1);
}

void ILI9488_FSMC::drawImage(const uint8_t* img, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    // rudimentary clipping (drawChar w/big text requires this)
    if((x >= _width) || (y >= _height)) return;
    if((x + w - 1) >= _width)  w = _width  - x;
    if((y + h - 1) >= _height) h = _height - y;
    setAddrWindow(x, y, x+w-1, y+h-1);
    uint16_t linebuff[w+1];
    uint16_t pixels = w*h;
    uint32_t count = 0;
    for (uint16_t i = 0; i < h; i++) {
	     uint16_t pixcount = 0;
		 for (uint16_t o = 0; o <  w; o++) {
		      uint8_t b1 = img[count];
			  count++;
			  uint8_t b2 = img[count];
			  count++;
			  uint16_t color = b1 << 8 | b2; // now have a 5-6-5 composite pixel
			  linebuff[pixcount] = (b1 << 8) + b2;
			  pixcount++;
         }
		 
      for(uint16_t b = 0; b < w; b++) {
	    ILI9488_RAM=linebuff[b];
      }
  }
}

void ILI9488_FSMC::pushColor(uint16_t color) {
  ILI9488_RAM=color;
}

void ILI9488_FSMC::pushColors(uint16_t *data, uint8_t len, boolean first) { 
  uint8_t lencount = len;
  while(lencount--) ILI9488_RAM=*data++;
}

void ILI9488_FSMC::write16BitColor(uint16_t color) {
  ILI9488_RAM=color;
}

void ILI9488_FSMC::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
  setAddrWindow(x,y,x+1,y+1);
  write16BitColor(color);
}

void ILI9488_FSMC::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((y+h-1) >= _height)
      h = _height-y;
  setAddrWindow(x, y, x, y+h-1);
  while (h--) {
    write16BitColor(color);
  }
}

void ILI9488_FSMC::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;
  setAddrWindow(x, y, x+w-1, y);
  while (w--) {
    write16BitColor(color);
  }
}

void ILI9488_FSMC::fillScreen(uint16_t color) {
  fillRect(0, 0,  _width, _height, color);
}

void ILI9488_FSMC::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;
  setAddrWindow(x, y, x+w-1, y+h-1);
  uint32_t count = w*h;
  while (count--) ILI9488_RAM=color;
}

uint16_t ILI9488_FSMC::color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void ILI9488_FSMC::setRotation(uint8_t m) {
  writecommand(ILI9488_MADCTL);
  rotation = m % 4; // can't be higher than 3
  switch (rotation) {
   case 0:
     writedata(MADCTL_MX | MADCTL_BGR);
     _width  = ILI9488_TFTWIDTH;
     _height = ILI9488_TFTHEIGHT;
     break;
   case 1:
     writedata(MADCTL_MV | MADCTL_BGR);
     _width  = ILI9488_TFTHEIGHT;
     _height = ILI9488_TFTWIDTH;
     break;
  case 2:
    writedata(MADCTL_MY | MADCTL_BGR);
     _width  = ILI9488_TFTWIDTH;
     _height = ILI9488_TFTHEIGHT;
    break;
   case 3:
     writedata(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
     _width  = ILI9488_TFTHEIGHT;
     _height = ILI9488_TFTWIDTH;
     break;
  }
}

void ILI9488_FSMC::invertDisplay(boolean i) {
  writecommand(i ? ILI9488_INVON : ILI9488_INVOFF);
}

