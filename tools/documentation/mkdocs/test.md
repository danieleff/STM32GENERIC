# Self tesing boards

To automatically check if the board works, there are tests in the library.
These work by connecting pins to 

## Generic STM32 Arduino API test

This test should work on almost all STM32 boards. 

1. From the arduino menu, select File/Examples/Examples for XXX/SelfTest
2. Connect PA0 to PA1 to test digital/analog/read/write/interrupt
3. Connect SPI MOSI to SPI MISO to test SPI
4. Select a free UART (not used for Serial) on the board, connect the RX to TX, set the SerialUartToTest macro in the sketch
5. Run the sketch

The result should be short blink on the board LED.
If there was a failure, it is shown as a long LED 

The results are also 
