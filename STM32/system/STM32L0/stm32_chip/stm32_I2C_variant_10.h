
const stm32_af_pin_list_type chip_af_i2c_scl [] = {
//I2C1
    { I2C1  , GPIOA, GPIO_PIN_9  , GPIO_AF6_I2C1  }, 
    { I2C1  , GPIOB, GPIO_PIN_6  , GPIO_AF1_I2C1  }, 
//I2C3
    { I2C3  , GPIOA, GPIO_PIN_8  , GPIO_AF7_I2C3  }, 
}; 

const stm32_af_pin_list_type chip_af_i2c_sda [] = {
//I2C1
    { I2C1  , GPIOA, GPIO_PIN_10 , GPIO_AF6_I2C1  }, 
    { I2C1  , GPIOB, GPIO_PIN_7  , GPIO_AF1_I2C1  }, 
//I2C3
    { I2C3  , GPIOB, GPIO_PIN_4  , GPIO_AF7_I2C3  }, 
}; 
