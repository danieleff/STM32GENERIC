
const stm32_af_pin_list_type chip_af_i2c_scl [] = {
//I2C1
    { I2C1  , GPIOB, GPIO_PIN_6  , AF__HAL_AFIO_REMAP_I2C1_DISABLE}, 
    { I2C1  , GPIOB, GPIO_PIN_8  , AF__HAL_AFIO_REMAP_I2C1_ENABLE}, 
//I2C2
    { I2C2  , GPIOB, GPIO_PIN_10 , AF_NO_REMAP    }, 
}; 

const stm32_af_pin_list_type chip_af_i2c_sda [] = {
//I2C1
    { I2C1  , GPIOB, GPIO_PIN_7  , AF__HAL_AFIO_REMAP_I2C1_DISABLE}, 
    { I2C1  , GPIOB, GPIO_PIN_9  , AF__HAL_AFIO_REMAP_I2C1_ENABLE}, 
//I2C2
    { I2C2  , GPIOB, GPIO_PIN_11 , AF_NO_REMAP    }, 
}; 
