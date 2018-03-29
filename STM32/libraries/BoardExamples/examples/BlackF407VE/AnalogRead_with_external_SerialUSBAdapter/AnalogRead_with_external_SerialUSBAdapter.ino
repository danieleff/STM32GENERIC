/*
  AnalogReadSerial with external USB-Serial-Adapter

  Hardware:
   microcontroller board: STM32F407VE (  512MB flash )

  Serial Pins:
   PA9 USART1 TX
   PA10 USART1 RX

  Analog Pin: PA1


  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial

  reworked and documented for STM32FF407 Black
  March 2018 by ChrisMicro

  ! ATTENTION !
  pressing the reset button could be necessary after flashing the MCU

*/

#define mySerialPort SerialUART1

//#define mySerialPort Serial // you can use Serial if you select Tools=>Serial Communication=>SerialUART1
//#define mySerialPort SerialUART2 // don't forgett to change the RX and TX pin if you use SerialUART2

void setup()
{
  mySerialPort.stm32SetRX( PA10 );
  mySerialPort.stm32SetTX( PA9 );

  mySerialPort.begin(115200);
}


void loop()
{
  int value = analogRead(PA1);

  mySerialPort.println(value);

  delay(10);
}
