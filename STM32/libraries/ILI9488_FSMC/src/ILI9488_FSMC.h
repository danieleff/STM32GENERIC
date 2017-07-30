#ifndef _ILI9488H_
#define _ILI9488H_

#include <Arduino.h>
#include <Adafruit_GFX.h>

#define ILI9488_REG              (*((volatile unsigned short *) 0x60000000)) /* RS = 0 */ //FSMC bank 1 memory address
#define ILI9488_RAM              (*((volatile unsigned short *) 0x60020000)) /* RS = 1 */ //FSMC bank 1 memory address + bit17 set (i.e. DATA Write

#define ILI9488_TFTWIDTH  320
#define ILI9488_TFTHEIGHT 480

#define ILI9488_NOP     	0x00
#define ILI9488_SWRESET 	0x01
#define ILI9488_RDDID   	0x04
#define ILI9488_RDDST   	0x09

#define ILI9488_SLPIN   	0x10
#define ILI9488_SLPOUT  	0x11
#define ILI9488_PTLON   	0x12
#define ILI9488_NORON   	0x13

#define ILI9488_RDMODE  	0x0A
#define ILI9488_RDMADCTL  	0x0B
#define ILI9488_RDPIXFMT  	0x0C
#define ILI9488_RDIMGFMT  	0x0D
#define ILI9488_RDSELFDIAG  0x0F

#define ILI9488_INVOFF  	0x20
#define ILI9488_INVON   	0x21
#define ILI9488_GAMMASET 	0x26
#define ILI9488_DISPOFF 	0x28
#define ILI9488_DISPON  	0x29

#define ILI9488_CASET   	0x2A
#define ILI9488_PASET   	0x2B
#define ILI9488_RAMWR   	0x2C
#define ILI9488_RAMRD   	0x2E

#define ILI9488_PTLAR   	0x30
#define ILI9488_MADCTL  	0x36
#define ILI9488_PIXFMT  	0x3A

#define ILI9488_FRMCTR1 	0xB1
#define ILI9488_FRMCTR2 	0xB2
#define ILI9488_FRMCTR3 	0xB3
#define ILI9488_INVCTR  	0xB4
#define ILI9488_DFUNCTR 	0xB6

#define ILI9488_PWCTR1  	0xC0
#define ILI9488_PWCTR2  	0xC1
#define ILI9488_PWCTR3  	0xC2
#define ILI9488_PWCTR4  	0xC3
#define ILI9488_PWCTR5  	0xC4
#define ILI9488_VMCTR1  	0xC5
#define ILI9488_VMCTR2  	0xC7

#define ILI9488_RDID1   	0xDA
#define ILI9488_RDID2   	0xDB
#define ILI9488_RDID3   	0xDC
#define ILI9488_RDID4   	0xDD

#define ILI9488_GMCTRP1 	0xE0
#define ILI9488_GMCTRN1 	0xE1

#define MADCTL_MY  			0x80
#define MADCTL_MX  			0x40
#define MADCTL_MV  			0x20
#define MADCTL_ML  			0x10
#define MADCTL_RGB 			0x00
#define MADCTL_BGR 			0x08
#define MADCTL_MH  			0x04

// Color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFD20      /* 255, 165,   0 */
#define TFT_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define TFT_PINK        0xF81F

#define TFT_DELAY 		0xFF

class ILI9488_FSMC : public Adafruit_GFX {
 public:
        ILI9488_FSMC(void);
		void 		begin(void);
		void        bl_on(void);
		void        bl_off(void);
		void 		writecommand(uint8_t c);
		void 		writedata(uint8_t d);
		void 		setScrollArea(uint16_t topFixedArea, uint16_t bottomFixedArea);
		void 		scroll(uint16_t pixels);
		void 		setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
		void		startAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
		void 		drawImage(const uint8_t* img, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
		void 		pushColor(uint16_t color);
		void 		pushColors(uint16_t *data, uint8_t len, boolean first);
		void 		write16BitColor(uint16_t color);
		void 		drawPixel(int16_t x, int16_t y, uint16_t color);
		void 		drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
		void 		drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
		void 		fillScreen(uint16_t color);
		void 		fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
		uint16_t 	color565(uint8_t r, uint8_t g, uint8_t b);
		void 		setRotation(uint8_t m);
		void 		invertDisplay(boolean i);
 private:
		uint32_t _cs, _wr, _dc, _rd, _bl, _rs;
		volatile uint32_t *csport,*wrport,*dcport,*rdport,*blport,*rsport;
		int32_t csmask,wrmask,dcmask,rdmask,blmask,rsmask;
};

#endif
