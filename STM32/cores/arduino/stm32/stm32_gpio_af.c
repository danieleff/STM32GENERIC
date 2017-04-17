#include "stm32_gpio_af.h"

#include CHIP_PERIPHERAL_INCLUDE

GPIO_TypeDef *stm32AfGetDefault(stm32_af_pin_list_type list[], int size, const void *instance, uint32_t *pin) {
    for(int i=0; i<size; i++) {
        if (instance == list[i].instance) {
            *pin = list[i].pin;
            return list[i].port;
        }
    }
    return NULL;
}

uint32_t stm32GetClockFrequency(void *instance) {
	for(int i=0; i<sizeof(chip_clock_freq_list) / sizeof(chip_clock_freq_list[0]); i++) {
		if (chip_clock_freq_list[i].instance == instance) {
			return (chip_clock_freq_list[i].clock_freq_func)();
		}
	}
	return 0;
}

uint8_t stm32ADC1GetChannel(GPIO_TypeDef *port, uint32_t pin_mask) {
    for(int i=0; i<sizeof(chip_adc1_channel) / sizeof(chip_adc1_channel[0]); i++) {
        if (chip_adc1_channel[i].port == port && chip_adc1_channel[i].pin_mask == pin_mask) {
            return chip_adc1_channel[i].channel;
        }
    }

    return -1;
}

void stm32AfUARTInit(const USART_TypeDef *instance,
    GPIO_TypeDef *rxPort, uint32_t rxPin,
    GPIO_TypeDef *txPort, uint32_t txPin) {

    stm32AfInit(chip_af_usart_rx, sizeof(chip_af_usart_rx) / sizeof(chip_af_usart_rx[0]), instance, rxPort, rxPin, GPIO_MODE_AF_PP, GPIO_PULLUP);
    stm32AfInit(chip_af_usart_tx, sizeof(chip_af_usart_tx) / sizeof(chip_af_usart_tx[0]), instance, txPort, txPin, GPIO_MODE_AF_PP, GPIO_PULLUP);

}

void stm32AfSPIInit(const SPI_TypeDef *instance,
    GPIO_TypeDef *mosiPort, uint32_t mosiPin,
    GPIO_TypeDef *misoPort, uint32_t misoPin,
	GPIO_TypeDef *sckPort, uint32_t sckPin) {

    stm32AfInit(chip_af_spi_mosi, sizeof(chip_af_spi_mosi) / sizeof(chip_af_spi_mosi[0]), instance, mosiPort, mosiPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_spi_miso, sizeof(chip_af_spi_miso) / sizeof(chip_af_spi_miso[0]), instance, misoPort, misoPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_spi_sck, sizeof(chip_af_spi_sck) / sizeof(chip_af_spi_sck[0]), instance, sckPort, sckPin, GPIO_MODE_AF_PP, GPIO_NOPULL);

}

void stm32AfI2CInit(const I2C_TypeDef *instance,
    GPIO_TypeDef *sdaPort, uint32_t sdaPin,
    GPIO_TypeDef *sclPort, uint32_t sclPin) {

    stm32AfInit(chip_af_i2c_sda, sizeof(chip_af_i2c_sda) / sizeof(chip_af_i2c_sda[0]), instance, sdaPort, sdaPin, GPIO_MODE_AF_OD, GPIO_PULLUP);
    stm32AfInit(chip_af_i2c_scl, sizeof(chip_af_i2c_scl) / sizeof(chip_af_i2c_scl[0]), instance, sclPort, sclPin, GPIO_MODE_AF_OD, GPIO_PULLUP);
}
