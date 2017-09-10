//LTDC for F429 Discovery board

#ifndef LTDC_F4_H
#define LTDC_F4_H

#include "LTDC.h"
#include "SPI.h"

// The display on STM32F429ZI Discovery
constexpr LTDCSettings LTDC_F429 = {
    .width = 240,
    .height = 320,
    .horizontalSync = 1,
    .horizontalFrontPorch = 0,
    .horizontalBackPorch = 0,
    .verticalSync = 1,
    .verticalFrontPorch = 0,
    .verticalBackPorch = 0,
};


// SPI Commands
#define LCD_SLEEP_OUT           0x11   /* Sleep out register */
#define LCD_GAMMA               0x26   /* Gamma register */
#define LCD_DISPLAY_OFF         0x28   /* Display off register */
#define LCD_DISPLAY_ON          0x29   /* Display on register */
#define LCD_COLUMN_ADDR         0x2A   /* Column address register */
#define LCD_PAGE_ADDR           0x2B   /* Page address register */
#define LCD_GRAM                0x2C   /* GRAM register */
#define LCD_MAC                 0x36   /* Memory Access Control register*/
#define LCD_RGB_INTERFACE       0xB0   /* RGB Interface Signal Control */
#define LCD_FRMCTR1             0xB1   /* Frame Rate Control (In Normal Mode) */
#define LCD_DFC                 0xB6   /* Display Function Control register */
#define LCD_POWER1              0xC0   /* Power Control 1 register */
#define LCD_POWER2              0xC1   /* Power Control 2 register */
#define LCD_VCOM1               0xC5   /* VCOM Control 1 register */
#define LCD_VCOM2               0xC7   /* VCOM Control 2 register */
#define LCD_PGAMMA              0xE0   /* Positive Gamma Correction register */
#define LCD_NGAMMA              0xE1   /* Negative Gamma Correction register */
#define LCD_DGAMCTRL1           0xE2   /* Digital Gamma Control 1 */
#define LCD_INTERFACE           0xF6   /* Interface control register */
#define LCD_POWERA              0xCB   /* Power control A register */
#define LCD_POWERB              0xCF   /* Power control B register */
#define LCD_DTCA                0xE8   /* Driver timing control A */
#define LCD_DTCB                0xEA   /* Driver timing control B */
#define LCD_POWER_SEQ           0xED   /* Power on sequence register */
#define LCD_3GAMMA_EN           0xF2   /* 3 Gamma enable register */
#define LCD_PRC                 0xF7   /* Pump ratio control register */

#define LCD_CS                  PD13
#define LCD_DC                  PC2

//Just for initialization
SPIClass SPI_ILI9341(SPI5, PF9, PF8, PF7);


class LTDC_F429_Discovery : public LTDCClass<LTDC_F429.width, LTDC_F429.height> {
    public:
    LTDC_F429_Discovery(): LTDCClass(LTDC_F429) {

    };

    void spiWriteCommand(uint8_t command) {
        digitalWrite(LCD_CS, LOW);
        digitalWrite(LCD_DC, LOW);
        SPI_ILI9341.transfer(command);
        digitalWrite(LCD_DC, HIGH);
    }
    void spiWriteData(uint8_t data) {
        digitalWrite(LCD_CS, HIGH);
        digitalWrite(LCD_DC, LOW);
        SPI_ILI9341.transfer(data);
        digitalWrite(LCD_DC, HIGH);
    }

    void init() {
        pinMode(LCD_CS, OUTPUT);
        pinMode(LCD_DC, OUTPUT);

        SPI_ILI9341.begin();
        SPI_ILI9341.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

        spiWriteCommand(0xCA);
        spiWriteData(0xC3);
        spiWriteData(0x08);
        spiWriteData(0x50);
        spiWriteCommand(LCD_POWERB);
        spiWriteData(0x00);
        spiWriteData(0xC1);
        spiWriteData(0x30);
        spiWriteCommand(LCD_POWER_SEQ);
        spiWriteData(0x64);
        spiWriteData(0x03);
        spiWriteData(0x12);
        spiWriteData(0x81);
        spiWriteCommand(LCD_DTCA);
        spiWriteData(0x85);
        spiWriteData(0x00);
        spiWriteData(0x78);
        spiWriteCommand(LCD_POWERA);
        spiWriteData(0x39);
        spiWriteData(0x2C);
        spiWriteData(0x00);
        spiWriteData(0x34);
        spiWriteData(0x02);
        spiWriteCommand(LCD_PRC);
        spiWriteData(0x20);
        spiWriteCommand(LCD_DTCB);
        spiWriteData(0x00);
        spiWriteData(0x00);
        spiWriteCommand(LCD_FRMCTR1);
        spiWriteData(0x00);
        spiWriteData(0x1B);
        spiWriteCommand(LCD_DFC);
        spiWriteData(0x0A);
        spiWriteData(0xA2);
        spiWriteCommand(LCD_POWER1);
        spiWriteData(0x10);
        spiWriteCommand(LCD_POWER2);
        spiWriteData(0x10);
        spiWriteCommand(LCD_VCOM1);
        spiWriteData(0x45);
        spiWriteData(0x15);
        spiWriteCommand(LCD_VCOM2);
        spiWriteData(0x90);
        spiWriteCommand(LCD_MAC);
        spiWriteData(0xC8);
        spiWriteCommand(LCD_3GAMMA_EN);
        spiWriteData(0x00);

        spiWriteCommand(LCD_RGB_INTERFACE);
        spiWriteData(0xC2);

        spiWriteCommand(LCD_DFC);
        spiWriteData(0x0A);
        spiWriteData(0xA7);
        spiWriteData(0x27);
        spiWriteData(0x04);

        /* Colomn address set */
        spiWriteCommand(LCD_COLUMN_ADDR);
        spiWriteData(0x00);
        spiWriteData(0x00);
        spiWriteData(0x00);
        spiWriteData(0xEF);
        /* Page address set */
        spiWriteCommand(LCD_PAGE_ADDR);
        spiWriteData(0x00);
        spiWriteData(0x00);
        spiWriteData(0x01);
        spiWriteData(0x3F);
        spiWriteCommand(LCD_INTERFACE);
        spiWriteData(0x01);
        spiWriteData(0x00);
        spiWriteData(0x06);

        spiWriteCommand(LCD_GRAM);
        delay(200);

        spiWriteCommand(LCD_GAMMA);
        spiWriteData(0x01);

        spiWriteCommand(LCD_PGAMMA);
        spiWriteData(0x0F);
        spiWriteData(0x29);
        spiWriteData(0x24);
        spiWriteData(0x0C);
        spiWriteData(0x0E);
        spiWriteData(0x09);
        spiWriteData(0x4E);
        spiWriteData(0x78);
        spiWriteData(0x3C);
        spiWriteData(0x09);
        spiWriteData(0x13);
        spiWriteData(0x05);
        spiWriteData(0x17);
        spiWriteData(0x11);
        spiWriteData(0x00);
        spiWriteCommand(LCD_NGAMMA);
        spiWriteData(0x00);
        spiWriteData(0x16);
        spiWriteData(0x1B);
        spiWriteData(0x04);
        spiWriteData(0x11);
        spiWriteData(0x07);
        spiWriteData(0x31);
        spiWriteData(0x33);
        spiWriteData(0x42);
        spiWriteData(0x05);
        spiWriteData(0x0C);
        spiWriteData(0x0A);
        spiWriteData(0x28);
        spiWriteData(0x2F);
        spiWriteData(0x0F);
        spiWriteCommand(LCD_SLEEP_OUT);
        delay(200);
        spiWriteCommand(LCD_DISPLAY_ON);

        spiWriteCommand(LCD_GRAM);

        static RCC_PeriphCLKInitTypeDef  periph_clk_init_struct;
        periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
        periph_clk_init_struct.PLLSAI.PLLSAIN = 192;
        periph_clk_init_struct.PLLSAI.PLLSAIR = 4;
        periph_clk_init_struct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
        HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);


        GPIO_InitTypeDef gpio_init_structure;

        /* Enable GPIOs clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        __HAL_RCC_GPIOF_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();

        /*** LTDC Pins configuration ***/
        /* GPIOA configuration */
        gpio_init_structure.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 |
                               GPIO_PIN_11 | GPIO_PIN_12;
        gpio_init_structure.Mode = GPIO_MODE_AF_PP;
        gpio_init_structure.Pull = GPIO_NOPULL;
        gpio_init_structure.Speed = GPIO_SPEED_FAST;
        gpio_init_structure.Alternate= GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOA, &gpio_init_structure);

        /* GPIOB configuration */
        gpio_init_structure.Pin = GPIO_PIN_8 | \
                               GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
        HAL_GPIO_Init(GPIOB, &gpio_init_structure);

        /* GPIOC configuration */
        gpio_init_structure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10;
        HAL_GPIO_Init(GPIOC, &gpio_init_structure);

        /* GPIOD configuration */
        gpio_init_structure.Pin = GPIO_PIN_3 | GPIO_PIN_6;
        HAL_GPIO_Init(GPIOD, &gpio_init_structure);

        /* GPIOF configuration */
        gpio_init_structure.Pin = GPIO_PIN_10;
        HAL_GPIO_Init(GPIOF, &gpio_init_structure);

        /* GPIOG configuration */
        gpio_init_structure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | \
                               GPIO_PIN_11;
        HAL_GPIO_Init(GPIOG, &gpio_init_structure);

        /* GPIOB configuration */
        gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        gpio_init_structure.Alternate= GPIO_AF9_LTDC;
        HAL_GPIO_Init(GPIOB, &gpio_init_structure);

        /* GPIOG configuration */
        gpio_init_structure.Pin = GPIO_PIN_10 | GPIO_PIN_12;
        HAL_GPIO_Init(GPIOG, &gpio_init_structure);

    }
};

#endif
