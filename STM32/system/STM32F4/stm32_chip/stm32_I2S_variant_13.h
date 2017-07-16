#define STM32_CHIP_HAS_I2S

const stm32_af_pin_list_type chip_af_i2s_ck [] = {
//I2S1
    { SPI1  , GPIOA, GPIO_PIN_5  , GPIO_AF5_SPI1  }, 
    { SPI1  , GPIOB, GPIO_PIN_3  , GPIO_AF5_SPI1  }, 
//I2S2
    { SPI2  , GPIOB, GPIO_PIN_10 , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOB, GPIO_PIN_13 , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOC, GPIO_PIN_7  , GPIO_AF5_SPI2  }, 
//I2S3
    { SPI3  , GPIOB, GPIO_PIN_3  , GPIO_AF6_SPI3  }, 
    { SPI3  , GPIOB, GPIO_PIN_12 , GPIO_AF5_SPI3  }, 
    { SPI3  , GPIOC, GPIO_PIN_10 , GPIO_AF6_SPI3  }, 
//I2S4
    { SPI4  , GPIOB, GPIO_PIN_13 , GPIO_AF6_SPI4  }, 
//I2S5
    { SPI5  , GPIOB, GPIO_PIN_0  , GPIO_AF6_SPI5  }, 
}; 

const stm32_af_pin_list_type chip_af_i2s_mck [] = {
//I2S1
    { SPI1  , GPIOC, GPIO_PIN_4  , GPIO_AF5_SPI1  }, 
//I2S2
    { SPI2  , GPIOA, GPIO_PIN_3  , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOA, GPIO_PIN_6  , GPIO_AF6_SPI2  }, 
    { SPI2  , GPIOC, GPIO_PIN_6  , GPIO_AF5_SPI2  }, 
//I2S3
    { SPI3  , GPIOB, GPIO_PIN_10 , GPIO_AF6_SPI3  }, 
    { SPI3  , GPIOC, GPIO_PIN_7  , GPIO_AF6_SPI3  }, 
}; 

const stm32_af_pin_list_type chip_af_i2s_sd [] = {
//I2S1
    { SPI1  , GPIOA, GPIO_PIN_7  , GPIO_AF5_SPI1  }, 
    { SPI1  , GPIOB, GPIO_PIN_5  , GPIO_AF5_SPI1  }, 
//I2S2
    { SPI2  , GPIOB, GPIO_PIN_15 , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOC, GPIO_PIN_3  , GPIO_AF5_SPI2  }, 
//I2S3
    { SPI3  , GPIOB, GPIO_PIN_5  , GPIO_AF6_SPI3  }, 
    { SPI3  , GPIOC, GPIO_PIN_12 , GPIO_AF6_SPI3  }, 
//I2S4
    { SPI4  , GPIOA, GPIO_PIN_1  , GPIO_AF5_SPI4  }, 
//I2S5
    { SPI5  , GPIOA, GPIO_PIN_10 , GPIO_AF6_SPI5  }, 
    { SPI5  , GPIOB, GPIO_PIN_8  , GPIO_AF6_SPI5  }, 
}; 

const stm32_af_pin_list_type chip_af_i2s_ws [] = {
//I2S1
    { SPI1  , GPIOA, GPIO_PIN_4  , GPIO_AF5_SPI1  }, 
    { SPI1  , GPIOA, GPIO_PIN_15 , GPIO_AF5_SPI1  }, 
//I2S2
    { SPI2  , GPIOB, GPIO_PIN_9  , GPIO_AF5_SPI2  }, 
    { SPI2  , GPIOB, GPIO_PIN_12 , GPIO_AF5_SPI2  }, 
//I2S3
    { SPI3  , GPIOA, GPIO_PIN_4  , GPIO_AF6_SPI3  }, 
    { SPI3  , GPIOA, GPIO_PIN_15 , GPIO_AF6_SPI3  }, 
//I2S4
    { SPI4  , GPIOB, GPIO_PIN_12 , GPIO_AF6_SPI4  }, 
//I2S5
    { SPI5  , GPIOB, GPIO_PIN_1  , GPIO_AF6_SPI5  }, 
}; 

const stm32_af_pin_list_type chip_af_i2s_ext_sd [] = {
//I2S2
    { SPI2  , GPIOB, GPIO_PIN_14 , GPIO_AF6_SPI2  }, 
    { SPI2  , GPIOC, GPIO_PIN_2  , GPIO_AF6_SPI2  }, 
//I2S3
    { SPI3  , GPIOB, GPIO_PIN_4  , GPIO_AF5_SPI3  }, 
    { SPI3  , GPIOC, GPIO_PIN_11 , GPIO_AF5_I2S3ext}, 
}; 
