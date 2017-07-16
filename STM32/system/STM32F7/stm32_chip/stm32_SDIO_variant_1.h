#define STM32_CHIP_HAS_SDIO

const stm32_af_pin_list_type chip_af_sdio_ck [] = {
//SDMMC1
    { SDMMC1, GPIOC, GPIO_PIN_12 , GPIO_AF12_SDMMC1}, 
//SDMMC2
    { SDMMC2, GPIOD, GPIO_PIN_6  , GPIO_AF11_SDMMC2}, 
}; 

const stm32_af_pin_list_type chip_af_sdio_cmd [] = {
//SDMMC1
    { SDMMC1, GPIOD, GPIO_PIN_2  , GPIO_AF12_SDMMC1}, 
//SDMMC2
    { SDMMC2, GPIOD, GPIO_PIN_7  , GPIO_AF11_SDMMC2}, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d0 [] = {
//SDMMC1
    { SDMMC1, GPIOC, GPIO_PIN_8  , GPIO_AF12_SDMMC1}, 
//SDMMC2
    { SDMMC2, GPIOB, GPIO_PIN_14 , GPIO_AF10_SDMMC2}, 
    { SDMMC2, GPIOG, GPIO_PIN_9  , GPIO_AF11_SDMMC2}, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d1 [] = {
//SDMMC1
    { SDMMC1, GPIOC, GPIO_PIN_9  , GPIO_AF12_SDMMC1}, 
//SDMMC2
    { SDMMC2, GPIOB, GPIO_PIN_15 , GPIO_AF10_SDMMC2}, 
    { SDMMC2, GPIOG, GPIO_PIN_10 , GPIO_AF11_SDMMC2}, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d2 [] = {
//SDMMC1
    { SDMMC1, GPIOC, GPIO_PIN_10 , GPIO_AF12_SDMMC1}, 
//SDMMC2
    { SDMMC2, GPIOB, GPIO_PIN_3  , GPIO_AF10_SDMMC2}, 
    { SDMMC2, GPIOG, GPIO_PIN_11 , GPIO_AF10_SDMMC2}, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d3 [] = {
//SDMMC1
    { SDMMC1, GPIOC, GPIO_PIN_11 , GPIO_AF12_SDMMC1}, 
//SDMMC2
    { SDMMC2, GPIOB, GPIO_PIN_4  , GPIO_AF10_SDMMC2}, 
    { SDMMC2, GPIOG, GPIO_PIN_12 , GPIO_AF11_SDMMC2}, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d4 [] = {
//SDMMC1
    { SDMMC1, GPIOB, GPIO_PIN_8  , GPIO_AF12_SDMMC1}, 
//SDMMC2
    { SDMMC2, GPIOB, GPIO_PIN_8  , GPIO_AF10_SDMMC2}, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d5 [] = {
//SDMMC1
    { SDMMC1, GPIOB, GPIO_PIN_9  , GPIO_AF12_SDMMC1}, 
//SDMMC2
    { SDMMC2, GPIOB, GPIO_PIN_9  , GPIO_AF10_SDMMC2}, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d6 [] = {
//SDMMC1
    { SDMMC1, GPIOC, GPIO_PIN_6  , GPIO_AF12_SDMMC1}, 
//SDMMC2
    { SDMMC2, GPIOC, GPIO_PIN_6  , GPIO_AF10_SDMMC2}, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d7 [] = {
//SDMMC1
    { SDMMC1, GPIOC, GPIO_PIN_7  , GPIO_AF12_SDMMC1}, 
//SDMMC2
    { SDMMC2, GPIOC, GPIO_PIN_7  , GPIO_AF10_SDMMC2}, 
}; 
