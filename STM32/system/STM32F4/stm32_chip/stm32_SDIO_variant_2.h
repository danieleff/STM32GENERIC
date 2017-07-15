#define STM32_CHIP_HAS_SDIO

const stm32_af_pin_list_type chip_af_sdio_ck [] = {
//SDIO
    { SDIO  , GPIOB, GPIO_PIN_15 , GPIO_AF12_SDIO }, 
}; 

const stm32_af_pin_list_type chip_af_sdio_cmd [] = {
//SDIO
    { SDIO  , GPIOA, GPIO_PIN_6  , GPIO_AF12_SDIO }, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d0 [] = {
//SDIO
    { SDIO  , GPIOB, GPIO_PIN_4  , GPIO_AF12_SDIO }, 
    { SDIO  , GPIOB, GPIO_PIN_7  , GPIO_AF12_SDIO }, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d1 [] = {
//SDIO
    { SDIO  , GPIOA, GPIO_PIN_8  , GPIO_AF12_SDIO }, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d2 [] = {
//SDIO
    { SDIO  , GPIOA, GPIO_PIN_9  , GPIO_AF12_SDIO }, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d3 [] = {
//SDIO
    { SDIO  , GPIOB, GPIO_PIN_5  , GPIO_AF12_SDIO }, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d4 [] = {
//SDIO
    { SDIO  , GPIOB, GPIO_PIN_8  , GPIO_AF12_SDIO }, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d5 [] = {
//SDIO
    { SDIO  , GPIOB, GPIO_PIN_9  , GPIO_AF12_SDIO }, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d6 [] = {
//SDIO
    { SDIO  , GPIOB, GPIO_PIN_14 , GPIO_AF12_SDIO }, 
}; 

const stm32_af_pin_list_type chip_af_sdio_d7 [] = {
//SDIO
    { SDIO  , GPIOB, GPIO_PIN_10 , GPIO_AF12_SDIO }, 
}; 
