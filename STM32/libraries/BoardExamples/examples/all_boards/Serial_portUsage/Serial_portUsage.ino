/*

 Serial Port usage and naming example.
 
 Most of the STM32 microcontrollers have more than one serial port.

 The STM32GENERIC Arduino framwork allows you to access this ports.

 The naming is

 Serial       = SerialUSB  this is equal to SerialUSB the virtual serial USB port
 SerialUART1               physical serial interface   
 SerialUART2               physical serial interface
 ...                       ...

 The SerialUARTx are in the data sheets sometimes called USARTx.
 Check the datasheed of your on board microcontroller to which pins this USARTS are connected.
 
 ! Do it in advance before programming the routine to avoid software induced short cuts !
 
 April 2017, ChrisMicro
 
 */

#define mySerialPort Serial          // equal to SerialUSB
//#define mySerialPort SerialUART1 
//#define mySerialPort SerialUART2

void setup() 
{
  mySerialPort.begin(115200);
  mySerialPort.println("Serial Port Example");
  mySerialPort.println("just counting up");
}

int Counter=0;

void loop() 
{
  mySerialPort.println(Counter); 
  Counter++;
  delay(1000);
}

