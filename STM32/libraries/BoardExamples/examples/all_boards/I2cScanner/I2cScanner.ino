
// -----------------------------------------------------
// i2c_scanner
//
// from // http://playground.arduino.cc/Main/I2cScanner
//
// This version is addapted to the STM32GENERIC library used 
// with a
// STM32F407 Discovery Board
// Some I2C devices have to be enabled first by digitalWrite
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//


#include <Wire.h>

#define SERIALINTERFACE Serial

#define AUDIOCODEC_CS PD4

void enableDevices()
{
  //on certain boards the I2C devices have to be enabled first
  // please put here your code to enable the devices
  // e.g. for the STM32F4 Discovery enabling the I2C codec is
  // pinMode(PD4,OUTPUT);
  // digitalWrite(PD4,HIGH);  
}

void setup()
{
  Wire.begin();
  
  enableDevices();

  SERIALINTERFACE.begin(9600);
  delay(5000);
  SERIALINTERFACE.println("\nI2C Scanner");
}


void loop()
{
  byte error, address;
  int nDevices;

  SERIALINTERFACE.println("Scanning...");
  delay(2000);

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    Wire.write(0);
    error = Wire.endTransmission();

    if (error == 0)
    {
      SERIALINTERFACE.print("I2C device found at address 0x");
      if (address < 16)  SERIALINTERFACE.print("0");
      SERIALINTERFACE.print(address, HEX);
      SERIALINTERFACE.println("  !");

      nDevices++;

    }
    else if (error == 4)
    {
      SERIALINTERFACE.print("no device found at address 0x");
      if (address < 16) SERIALINTERFACE.print("0");
      SERIALINTERFACE.println(address, HEX);
    }
  }
  if (nDevices == 0)
  {
    SERIALINTERFACE.println("No I2C devices found\n");
    SERIALINTERFACE.println("Did you configure the chip select for your device?\n");
  }
  else
    SERIALINTERFACE.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
}

