#include "stm32_gpio_af.h"

#include CHIP_PERIPHERAL_INCLUDE

GPIO_TypeDef *stm32_af_default(stm32_af_pin_list_type list[], int size, const void *instance, uint32_t *pin) {
    for(int i=0; i<size; i++) {
        if (instance == list[i].instance) {
            *pin = list[i].pin;
            return list[i].port;
        }
    }
    return NULL;
}

uint32_t stm32_apb_clock_freq(void *instance) {
	for(int i=0; i<sizeof(stm32_clock_freq_list) / sizeof(stm32_clock_freq_list[0]); i++) {
		if (stm32_clock_freq_list[i].instance == instance) {
			return (stm32_clock_freq_list[i].clock_freq_func)();
		}
	}
	return 0;
}

void stm32_af_uart_init(const USART_TypeDef *instance,
    GPIO_TypeDef *rxPort, uint32_t rxPin,
    GPIO_TypeDef *txPort, uint32_t txPin) {

    stm32_af_init(chip_af_usart_rx, sizeof(chip_af_usart_rx) / sizeof(chip_af_usart_rx[0]), instance, rxPort, rxPin, GPIO_MODE_AF_PP, GPIO_PULLUP);
    stm32_af_init(chip_af_usart_tx, sizeof(chip_af_usart_tx) / sizeof(chip_af_usart_tx[0]), instance, txPort, txPin, GPIO_MODE_AF_PP, GPIO_PULLUP);

}

void stm32_af_spi_init(const SPI_TypeDef *instance,
    GPIO_TypeDef *mosiPort, uint32_t mosiPin,
    GPIO_TypeDef *misoPort, uint32_t misoPin,
	GPIO_TypeDef *sckPort, uint32_t sckPin) {

    stm32_af_init(chip_af_spi_mosi, sizeof(chip_af_spi_mosi) / sizeof(chip_af_spi_mosi[0]), instance, mosiPort, mosiPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32_af_init(chip_af_spi_miso, sizeof(chip_af_spi_miso) / sizeof(chip_af_spi_miso[0]), instance, misoPort, misoPin, GPIO_MODE_AF_PP, GPIO_NOPULL);
    stm32_af_init(chip_af_spi_sck, sizeof(chip_af_spi_sck) / sizeof(chip_af_spi_sck[0]), instance, sckPort, sckPin, GPIO_MODE_AF_PP, GPIO_NOPULL);

}
