#define STM32_CHIP_HAS_I2S

const stm32_af_pin_list_type chip_af_i2s_ck [] = {
//I2S2
    { SPI2  , GPIOB, GPIO_PIN_13 , AF_NO_REMAP    }, 
//I2S3
    { SPI3  , GPIOB, GPIO_PIN_3  , AF__HAL_AFIO_REMAP_I2S3_DISABLE}, 
}; 

const stm32_af_pin_list_type chip_af_i2s_mck [] = {
//I2S2
    { SPI2  , GPIOC, GPIO_PIN_6  , AF_NO_REMAP    }, 
//I2S3
    { SPI3  , GPIOC, GPIO_PIN_7  , AF_NO_REMAP    }, 
}; 

const stm32_af_pin_list_type chip_af_i2s_sd [] = {
//I2S2
    { SPI2  , GPIOB, GPIO_PIN_15 , AF_NO_REMAP    }, 
//I2S3
    { SPI3  , GPIOB, GPIO_PIN_5  , AF__HAL_AFIO_REMAP_I2S3_DISABLE}, 
}; 

const stm32_af_pin_list_type chip_af_i2s_ws [] = {
//I2S2
    { SPI2  , GPIOB, GPIO_PIN_12 , AF_NO_REMAP    }, 
//I2S3
    { SPI3  , GPIOA, GPIO_PIN_15 , AF__HAL_AFIO_REMAP_I2S3_DISABLE}, 
}; 
