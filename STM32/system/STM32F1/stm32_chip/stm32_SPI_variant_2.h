
const stm32_af_pin_list_type chip_af_spi_miso [] = {
//SPI1
    { SPI1  , GPIOA, GPIO_PIN_6  , AF__HAL_AFIO_REMAP_SPI1_DISABLE}, 
    { SPI1  , GPIOB, GPIO_PIN_4  , AF__HAL_AFIO_REMAP_SPI1_ENABLE}, 
//SPI2
    { SPI2  , GPIOB, GPIO_PIN_14 , AF_NO_REMAP    }, 
}; 

const stm32_af_pin_list_type chip_af_spi_mosi [] = {
//SPI1
    { SPI1  , GPIOA, GPIO_PIN_7  , AF__HAL_AFIO_REMAP_SPI1_DISABLE}, 
    { SPI1  , GPIOB, GPIO_PIN_5  , AF__HAL_AFIO_REMAP_SPI1_ENABLE}, 
//SPI2
    { SPI2  , GPIOB, GPIO_PIN_15 , AF_NO_REMAP    }, 
}; 

const stm32_af_pin_list_type chip_af_spi_nss [] = {
//SPI1
    { SPI1  , GPIOA, GPIO_PIN_4  , AF__HAL_AFIO_REMAP_SPI1_DISABLE}, 
    { SPI1  , GPIOA, GPIO_PIN_15 , AF__HAL_AFIO_REMAP_SPI1_ENABLE}, 
//SPI2
    { SPI2  , GPIOB, GPIO_PIN_12 , AF_NO_REMAP    }, 
}; 

const stm32_af_pin_list_type chip_af_spi_sck [] = {
//SPI1
    { SPI1  , GPIOA, GPIO_PIN_5  , AF__HAL_AFIO_REMAP_SPI1_DISABLE}, 
    { SPI1  , GPIOB, GPIO_PIN_3  , AF__HAL_AFIO_REMAP_SPI1_ENABLE}, 
//SPI2
    { SPI2  , GPIOB, GPIO_PIN_13 , AF_NO_REMAP    }, 
}; 
