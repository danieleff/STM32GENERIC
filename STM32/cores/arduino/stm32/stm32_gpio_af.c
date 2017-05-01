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

#ifdef STM32_CHIP_HAS_I2S

void stm32AfI2SInit(const SPI_TypeDef *instance,
    GPIO_TypeDef *sdPort, uint32_t sdPin,
    GPIO_TypeDef *wsPort, uint32_t wsPin,
    GPIO_TypeDef *ckPort, uint32_t ckPin) {

    stm32AfInit(chip_af_i2s_sd, sizeof(chip_af_i2s_sd) / sizeof(chip_af_i2s_sd[0]), instance, sdPort, sdPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_i2s_ws, sizeof(chip_af_i2s_ws) / sizeof(chip_af_i2s_ws[0]), instance, wsPort, wsPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_i2s_ck, sizeof(chip_af_i2s_ck) / sizeof(chip_af_i2s_ck[0]), instance, ckPort, ckPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
}

void stm32AfI2SInitWithMck(const SPI_TypeDef *instance,
    GPIO_TypeDef *sdPort, uint32_t sdPin,
    GPIO_TypeDef *wsPort, uint32_t wsPin,
    GPIO_TypeDef *ckPort, uint32_t ckPin,
    GPIO_TypeDef *mckPort, uint32_t mckPin) {

    stm32AfInit(chip_af_i2s_sd, sizeof(chip_af_i2s_sd) / sizeof(chip_af_i2s_sd[0]), instance, sdPort, sdPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_i2s_ws, sizeof(chip_af_i2s_ws) / sizeof(chip_af_i2s_ws[0]), instance, wsPort, wsPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_i2s_ck, sizeof(chip_af_i2s_ck) / sizeof(chip_af_i2s_ck[0]), instance, ckPort, ckPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_i2s_mck, sizeof(chip_af_i2s_mck) / sizeof(chip_af_i2s_mck[0]), instance, mckPort, mckPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
}

#endif

void stm32AfI2CInit(const I2C_TypeDef *instance,
    GPIO_TypeDef *sdaPort, uint32_t sdaPin,
    GPIO_TypeDef *sclPort, uint32_t sclPin) {

    stm32AfInit(chip_af_i2c_sda, sizeof(chip_af_i2c_sda) / sizeof(chip_af_i2c_sda[0]), instance, sdaPort, sdaPin, GPIO_MODE_AF_OD, GPIO_PULLUP);
    stm32AfInit(chip_af_i2c_scl, sizeof(chip_af_i2c_scl) / sizeof(chip_af_i2c_scl[0]), instance, sclPort, sclPin, GPIO_MODE_AF_OD, GPIO_PULLUP);
}

#ifdef STM32_CHIP_HAS_SDIO

void stm32AfSDIO1BitInit(const SD_TypeDef *instance,
    GPIO_TypeDef *ckPort, uint32_t ckPin,
    GPIO_TypeDef *cmdPort, uint32_t cmdPin,
    GPIO_TypeDef *d0Port, uint32_t d0Pin
    ) {

    stm32AfInit(chip_af_sdio_ck, sizeof(chip_af_sdio_ck) / sizeof(chip_af_sdio_ck[0]), instance, ckPort, ckPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_cmd, sizeof(chip_af_sdio_cmd) / sizeof(chip_af_sdio_cmd[0]), instance, cmdPort, cmdPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d0, sizeof(chip_af_sdio_d0) / sizeof(chip_af_sdio_d0[0]), instance, d0Port, d0Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
}

void stm32AfSDIO4BitInit(const SD_TypeDef *instance,
    GPIO_TypeDef *ckPort, uint32_t ckPin,
    GPIO_TypeDef *cmdPort, uint32_t cmdPin,
    GPIO_TypeDef *d0Port, uint32_t d0Pin,
    GPIO_TypeDef *d1Port, uint32_t d1Pin,
    GPIO_TypeDef *d2Port, uint32_t d2Pin,
    GPIO_TypeDef *d3Port, uint32_t d3Pin
    ) {

    stm32AfInit(chip_af_sdio_ck, sizeof(chip_af_sdio_ck) / sizeof(chip_af_sdio_ck[0]), instance, ckPort, ckPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_cmd, sizeof(chip_af_sdio_cmd) / sizeof(chip_af_sdio_cmd[0]), instance, cmdPort, cmdPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d0, sizeof(chip_af_sdio_d0) / sizeof(chip_af_sdio_d0[0]), instance, d0Port, d0Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d1, sizeof(chip_af_sdio_d1) / sizeof(chip_af_sdio_d1[0]), instance, d1Port, d1Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d2, sizeof(chip_af_sdio_d2) / sizeof(chip_af_sdio_d2[0]), instance, d2Port, d2Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d3, sizeof(chip_af_sdio_d3) / sizeof(chip_af_sdio_d3[0]), instance, d3Port, d3Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
}

void stm32AfSDIO8BitInit(const SD_TypeDef *instance,
    GPIO_TypeDef *ckPort, uint32_t ckPin,
    GPIO_TypeDef *cmdPort, uint32_t cmdPin,
    GPIO_TypeDef *d0Port, uint32_t d0Pin,
    GPIO_TypeDef *d1Port, uint32_t d1Pin,
    GPIO_TypeDef *d2Port, uint32_t d2Pin,
    GPIO_TypeDef *d3Port, uint32_t d3Pin,
    GPIO_TypeDef *d4Port, uint32_t d4Pin,
    GPIO_TypeDef *d5Port, uint32_t d5Pin,
    GPIO_TypeDef *d6Port, uint32_t d6Pin,
    GPIO_TypeDef *d7Port, uint32_t d7Pin
    ) {

    stm32AfInit(chip_af_sdio_ck, sizeof(chip_af_sdio_ck) / sizeof(chip_af_sdio_ck[0]), instance, ckPort, ckPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_cmd, sizeof(chip_af_sdio_cmd) / sizeof(chip_af_sdio_cmd[0]), instance, cmdPort, cmdPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d0, sizeof(chip_af_sdio_d0) / sizeof(chip_af_sdio_d0[0]), instance, d0Port, d0Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d1, sizeof(chip_af_sdio_d1) / sizeof(chip_af_sdio_d1[0]), instance, d1Port, d1Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d2, sizeof(chip_af_sdio_d2) / sizeof(chip_af_sdio_d2[0]), instance, d2Port, d2Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d3, sizeof(chip_af_sdio_d3) / sizeof(chip_af_sdio_d3[0]), instance, d3Port, d3Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d4, sizeof(chip_af_sdio_d4) / sizeof(chip_af_sdio_d4[0]), instance, d4Port, d4Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d5, sizeof(chip_af_sdio_d5) / sizeof(chip_af_sdio_d5[0]), instance, d5Port, d5Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d6, sizeof(chip_af_sdio_d6) / sizeof(chip_af_sdio_d6[0]), instance, d6Port, d6Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32AfInit(chip_af_sdio_d7, sizeof(chip_af_sdio_d7) / sizeof(chip_af_sdio_d7[0]), instance, d7Port, d7Pin, GPIO_MODE_AF_PP, GPIO_NOPULL);
}

#endif
