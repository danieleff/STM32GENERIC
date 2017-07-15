#define STM32_CHIP_HAS_I2S

const stm32_af_pin_list_type chip_af_i2s_ck [] = {
//I2S1
    { SPI1  , GPIOA, GPIO_PIN_5  , GPIO_AF0_SPI1  }, 
}; 

const stm32_af_pin_list_type chip_af_i2s_mck [] = {
//I2S1
    { SPI1  , GPIOA, GPIO_PIN_6  , GPIO_AF0_SPI1  }, 
}; 

const stm32_af_pin_list_type chip_af_i2s_sd [] = {
//I2S1
    { SPI1  , GPIOA, GPIO_PIN_7  , GPIO_AF0_SPI1  }, 
}; 

const stm32_af_pin_list_type chip_af_i2s_ws [] = {
//I2S1
    { SPI1  , GPIOA, GPIO_PIN_4  , GPIO_AF0_SPI1  }, 
}; 
