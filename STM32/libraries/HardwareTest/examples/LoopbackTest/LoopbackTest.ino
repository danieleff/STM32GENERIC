/**
* Check the testXXX functions, and connect the given pins (PA0-PA1, SPI1 MOSI-MISO, UART1 RX-TX) with a wire
* to run the loopback self-tests.
* 
* Short blink: a test group passed
* Long blink: a test group failed
* 
* The result is also available on serial
*/

#include "Arduino.h"
#include "HardwareTest.h"

#include <SPI.h>

HardwareTest tester;

TestGroup gpioTest("GPIO", &tester);
TestGroup spiTest("SPI", &tester);
TestGroup uartTest("UART", &tester);

void testGPIO() {
  int pinOut = PA0;
  int pinIn = PA1;

  pinMode(pinOut, INPUT);
  pinMode(pinIn, INPUT_PULLUP);

  gpioTest.begin();

  gpioTest.assert(digitalRead(pinIn) == HIGH, "digitalRead(%i) should be HIGH (%i, %i not connected)", pinIn, pinOut, pinIn);

  pinMode(pinOut, OUTPUT);
  
  digitalWrite(pinOut, LOW);
  gpioTest.assert(digitalRead(pinIn) == LOW, "digitalRead(%i) should be LOW (%i - %i not connected)", pinIn, pinOut, pinIn);

  digitalWrite(pinOut, HIGH);
  gpioTest.assert(digitalRead(pinIn) == HIGH, "digitalRead(%i) should be HIGH (%i - %i not connected)", pinIn, pinOut, pinIn);

  gpioTest.end();
}

void testSPI() {
  spiTest.begin();

  const uint8_t data = 0x61;

  SPI.begin();
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  uint8_t read = SPI.transfer(data);

  spiTest.assert(read == data, "SPI.transfer(%i) == %i (SPI1 MISO - MOSI not connected)", data, read);

  SPI.endTransaction();
  SPI.end();

  spiTest.end();
}

void testUART(SerialUART &SerialUARTToTest) {
  uartTest.begin();

  uint8_t data = 0x61;

  SerialUARTToTest.begin(9600);
  SerialUARTToTest.write(data);
  delay(10);
  int read = SerialUARTToTest.read();

  uartTest.assert(read == data, "Serial1.read() = %i, should be %i (UART RX - TX not connected)", read, data);
  uartTest.end();
}

void setup() {
	Serial.begin(115200);
	tester.setLedPin(LED_BUILTIN, true);

}

void loop() {

	tester.begin();

	testGPIO();
	testSPI();
	testUART(SerialUART1);
	testUART(SerialUART2);

	tester.end();

	delay(2000);
}
