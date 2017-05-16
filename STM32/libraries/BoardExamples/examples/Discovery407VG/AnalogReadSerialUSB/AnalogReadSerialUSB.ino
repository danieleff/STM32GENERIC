/*
  AnalogReadSerialUSB

  To use this example you need to connect a second USB cable to your board.
  E.g. on the STM32F4 Discovery there is a second connector with a micro USB header.
  
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin 0, and the outside pins to +5V and ground.


  This example code is in the public domain.
*/

#define ANALOGPIN  PA1

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  SerialUSB.begin(115200);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(ANALOGPIN);
  // print out the value you read:
  SerialUSB.println(sensorValue);
  delay(10);        // delay in between reads for stability
}
