//LTDC for F746 Discovery board

#ifndef LTDC_F7_H
#define LTDC_F7_H

#include "LTDC.h"

// The display on STM32F746NG Discovery
constexpr LTDCSettings LTDC_F746_ROKOTECH = {
    .width = 480,
    .height = 272,
    .horizontalSync = 41,
    .horizontalFrontPorch = 32,
    .horizontalBackPorch = 13,
    .verticalSync = 10,
    .verticalFrontPorch = 2,
    .verticalBackPorch = 10,
};

class LTDC_F746_Discovery : public LTDCClass<480, 272> {
    public:
    LTDC_F746_Discovery(): LTDCClass(LTDC_F746_ROKOTECH) {

    };

    void init() {
        static RCC_PeriphCLKInitTypeDef  periph_clk_init_struct;
        periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
        periph_clk_init_struct.PLLSAI.PLLSAIN = 192;
        periph_clk_init_struct.PLLSAI.PLLSAIR = 5;
        periph_clk_init_struct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
        HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);


        GPIO_InitTypeDef gpio_init_structure;

        /* Enable GPIOs clock */
        __HAL_RCC_GPIOE_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();
        __HAL_RCC_GPIOI_CLK_ENABLE();
        __HAL_RCC_GPIOJ_CLK_ENABLE();
        __HAL_RCC_GPIOK_CLK_ENABLE();

        /*** LTDC Pins configuration ***/
        /* GPIOE configuration */
        gpio_init_structure.Pin       = GPIO_PIN_4;
        gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
        gpio_init_structure.Pull      = GPIO_NOPULL;
        gpio_init_structure.Speed     = GPIO_SPEED_FAST;
        gpio_init_structure.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOE, &gpio_init_structure);

        /* GPIOG configuration */
        gpio_init_structure.Pin       = GPIO_PIN_12;
        gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
        gpio_init_structure.Alternate = GPIO_AF9_LTDC;
        HAL_GPIO_Init(GPIOG, &gpio_init_structure);

        /* GPIOI LTDC alternate configuration */
        gpio_init_structure.Pin       = GPIO_PIN_9 | GPIO_PIN_10 | \
                                      GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
        gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
        gpio_init_structure.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOI, &gpio_init_structure);

        /* GPIOJ configuration */
        gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                      GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
                                      GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                                      GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
        gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
        gpio_init_structure.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

        /* GPIOK configuration */
        gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | \
                                      GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
        gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
        gpio_init_structure.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOK, &gpio_init_structure);

        // Display enable
        pinMode(PI12, OUTPUT);
        digitalWrite(PI12, HIGH);

        // Backlight enable
        pinMode(PK3, OUTPUT);
        digitalWrite(PK3, HIGH);


    }
};

#endif
