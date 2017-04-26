#ifndef SERIAL_UART_H
#define SERIAL_UART_H

#include "stm32_def.h"
#include "stm32_gpio.h"
#include "Stream.h"

#define BUFFER_SIZE 128

class SerialUART : public Stream  {
  public:
    SerialUART(USART_TypeDef *instance);
    void begin(const uint32_t baud);
    void end(void);
    int available(void);
    int peek(void);
    int read(void);
    void flush(void);
    size_t write(const uint8_t c);
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool() { return true; }; // UART always active

    void stm32SetRX(uint8_t rx);
    void stm32SetTX(uint8_t tx);
    
    USART_TypeDef *instance = NULL;
    UART_HandleTypeDef *handle = NULL;
    
    uint8_t receive_buffer = 0;

    uint8_t *txBuffer = NULL;
    volatile uint8_t txStart = 0;
    volatile uint8_t txEnd = 0;

    uint8_t *rxBuffer = NULL;
    volatile uint8_t rxStart = 0;
    volatile uint8_t rxEnd = 0;

    GPIO_TypeDef *rxPort = NULL;
    uint32_t rxPin = 0;
    GPIO_TypeDef *txPort = NULL;
    uint32_t txPin = 0;
};

#ifdef USART1
extern SerialUART SerialUART1;
#define Serial1 SerialUART1
#endif

#ifdef USART2
extern SerialUART SerialUART2;
#define Serial2 SerialUART2
#endif

#ifdef USART3
extern SerialUART SerialUART3;
#define Serial3 SerialUART3
#endif

#ifdef USART4
extern SerialUART SerialUART4;
#define Serial4 SerialUART4
#endif

#endif // _UART_CLASS_
