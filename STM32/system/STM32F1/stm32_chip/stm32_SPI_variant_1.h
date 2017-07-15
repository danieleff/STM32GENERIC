
const stm32_af_pin_list_type chip_af_spi_miso [] = {
//SPI1
    { SPI1  , GPIOA, GPIO_PIN_6  , AF__HAL_AFIO_REMAP_SPI1_DISABLE}, 
    { SPI1  , GPIOB, GPIO_PIN_4  , AF__HAL_AFIO_REMAP_SPI1_ENABLE}, 
}; 

const stm32_af_pin_list_type chip_af_spi_mosi [] = {
//SPI1
    { SPI1  , GPIOA, GPIO_PIN_7  , AF__HAL_AFIO_REMAP_SPI1_DISABLE}, 
    { SPI1  , GPIOB, GPIO_PIN_5  , AF__HAL_AFIO_REMAP_SPI1_ENABLE}, 
}; 

const stm32_af_pin_list_type chip_af_spi_nss [] = {
//SPI1
    { SPI1  , GPIOA, GPIO_PIN_4  , AF__HAL_AFIO_REMAP_SPI1_DISABLE}, 
    { SPI1  , GPIOA, GPIO_PIN_15 , AF__HAL_AFIO_REMAP_SPI1_ENABLE}, 
}; 

const stm32_af_pin_list_type chip_af_spi_sck [] = {
//SPI1
    { SPI1  , GPIOA, GPIO_PIN_5  , AF__HAL_AFIO_REMAP_SPI1_DISABLE}, 
    { SPI1  , GPIOB, GPIO_PIN_3  , AF__HAL_AFIO_REMAP_SPI1_ENABLE}, 
}; 
