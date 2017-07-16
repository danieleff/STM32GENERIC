#define STM32_CHIP_HAS_I2S

const stm32_af_pin_list_type chip_af_i2s_ck [] = {
//I2S2
    { SPI2  , GPIOB, GPIO_PIN_10 , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOB, GPIO_PIN_13 , GPIO_AF5_SPI2  }, 
//I2S3
    { SPI3  , GPIOB, GPIO_PIN_3  , GPIO_AF6_SPI3  }, 
    { SPI3  , GPIOC, GPIO_PIN_10 , GPIO_AF6_SPI3  }, 
}; 

const stm32_af_pin_list_type chip_af_i2s_mck [] = {
//I2S2
    { SPI2  , GPIOC, GPIO_PIN_6  , GPIO_AF5_SPI2  }, 
//I2S3
    { SPI3  , GPIOC, GPIO_PIN_7  , GPIO_AF6_SPI3  }, 
}; 

const stm32_af_pin_list_type chip_af_i2s_sd [] = {
//I2S2
    { SPI2  , GPIOB, GPIO_PIN_15 , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOC, GPIO_PIN_3  , GPIO_AF5_SPI2  }, 
//I2S3
    { SPI3  , GPIOB, GPIO_PIN_5  , GPIO_AF6_SPI3  }, 
    { SPI3  , GPIOC, GPIO_PIN_12 , GPIO_AF6_SPI3  }, 
}; 

const stm32_af_pin_list_type chip_af_i2s_ws [] = {
//I2S2
    { SPI2  , GPIOB, GPIO_PIN_9  , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOB, GPIO_PIN_12 , GPIO_AF5_SPI2  }, 
//I2S3
    { SPI3  , GPIOA, GPIO_PIN_4  , GPIO_AF6_SPI3  }, 
    { SPI3  , GPIOA, GPIO_PIN_15 , GPIO_AF6_SPI3  }, 
}; 

const stm32_af_pin_list_type chip_af_i2s_ext_sd [] = {
//I2S2
    { SPI2  , GPIOB, GPIO_PIN_14 , GPIO_AF6_I2S2ext}, 
    { SPI2  , GPIOC, GPIO_PIN_2  , GPIO_AF6_I2S2ext}, 
//I2S3
    { SPI3  , GPIOB, GPIO_PIN_4  , GPIO_AF7_I2S3ext}, 
    { SPI3  , GPIOC, GPIO_PIN_11 , GPIO_AF5_I2S3ext}, 
}; 
