
const stm32_af_pin_list_type chip_af_spi_miso [] = {
//SPI1
    { SPI1  , GPIOA, GPIO_PIN_6  , GPIO_AF0_SPI1  }, 
    { SPI1  , GPIOA, GPIO_PIN_11 , GPIO_AF0_SPI1  }, 
    { SPI1  , GPIOB, GPIO_PIN_0  , GPIO_AF1_SPI1  }, 
    { SPI1  , GPIOB, GPIO_PIN_4  , GPIO_AF0_SPI1  }, 
}; 

const stm32_af_pin_list_type chip_af_spi_mosi [] = {
//SPI1
    { SPI1  , GPIOA, GPIO_PIN_7  , GPIO_AF0_SPI1  }, 
    { SPI1  , GPIOA, GPIO_PIN_12 , GPIO_AF0_SPI1  }, 
    { SPI1  , GPIOB, GPIO_PIN_1  , GPIO_AF1_SPI1  }, 
    { SPI1  , GPIOB, GPIO_PIN_5  , GPIO_AF0_SPI1  }, 
}; 

const stm32_af_pin_list_type chip_af_spi_nss [] = {
//SPI1
    { SPI1  , GPIOA, GPIO_PIN_4  , GPIO_AF0_SPI1  }, 
    { SPI1  , GPIOA, GPIO_PIN_15 , GPIO_AF0_SPI1  }, 
}; 

const stm32_af_pin_list_type chip_af_spi_sck [] = {
//SPI1
    { SPI1  , GPIOA, GPIO_PIN_5  , GPIO_AF0_SPI1  }, 
    { SPI1  , GPIOB, GPIO_PIN_3  , GPIO_AF0_SPI1  }, 
}; 
