// LIS203DL: MEMS digital output motion sensor:
// ultra-low-power high-performance three-axis "nano" accelerometer
// read accelaration sensor and print serial
// STM32F4 Discovery
// accelerometer: LIS203DL ( included in STM32F4, some boards may have the LIS3DSH ) 
//
// April 2017, ChrisMicro

#include <SPI.h> 

#define SERIALINTERFACE SerialUSB

#define CS PE3

void accelerometer_SPI_select()
{
  digitalWrite(CS, LOW); // 0=SPI communication mode, 1=SPI IDLE MODE
  delayMicroseconds(50);
}

void accelerometer_SPI_release()
{
  digitalWrite(CS, HIGH);
  delayMicroseconds(50);
}

#define READREGISTERCOMMAND 0x80
uint8_t spiRead(uint8_t registerAddress)
{
  uint8_t r;
  
  accelerometer_SPI_select();
  SPI.transfer( registerAddress | READREGISTERCOMMAND );
  delayMicroseconds(500);
  r =  SPI.transfer(0); // dummy write, data read
  delayMicroseconds(500);
  accelerometer_SPI_release();
  
  return r;
}

void setup() 
{
  SERIALINTERFACE.begin(9600);

  while(SERIALINTERFACE.available()==0)
  {
      SERIALINTERFACE.println("\npess any key to start");
      SERIALINTERFACE.println("\n( on Arduino Serial Monitor press space the button 'send' on the right )");
      SERIALINTERFACE.println("");
      delay(1000);
  }
  SERIALINTERFACE.println("\n accelerometer");

  // configure SPI
  pinMode(CS, OUTPUT);

  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);

  // print sensor informatiion
  SERIALINTERFACE.print("INFO1    register: ");  SERIALINTERFACE.println( spiRead(0x0D), HEX);
  SERIALINTERFACE.print("INFO2    register: ");  SERIALINTERFACE.println( spiRead(0x0E), HEX);
  uint8_t whoAmI=spiRead(0x0F);
  SERIALINTERFACE.print("WHO AM I register: ");  SERIALINTERFACE.print( whoAmI , HEX);

  if(whoAmI==0x3B) SERIALINTERFACE.println("  LIS3DSL sensor,detected ");  
  if(whoAmI==0x3F) SERIALINTERFACE.println("  LIS3DSH sensor detected "); 

  SERIALINTERFACE.println("");

  // configure sensor
  accelerometer_SPI_select();
  SPI.transfer(0x20); // CTRL4 ODR register
  delay(10);
  SPI.transfer(0x47); // 25 Hz update, enable x,y,z
  accelerometer_SPI_release(); 
}

void loop() 
{
  int32_t x, y, xx, yy, z, zz;
  int16_t xg, yg, zg;
  
  x  = spiRead(0x28);
  xx = spiRead(0x29);
  y  = spiRead(0x2a);
  yy = spiRead(0x2b);
  z  = spiRead(0x2c);
  zz = spiRead(0x2d);

  xg = (x + (xx << 8));
  yg = (y + (yy << 8));
  zg = (z + (zz << 8));

  SERIALINTERFACE.print("X: ");
  SERIALINTERFACE.print(xg);

  SERIALINTERFACE.print(" Y: ");
  SERIALINTERFACE.print( yg);

  SERIALINTERFACE.print(" Z: ");
  SERIALINTERFACE.println(zg);

  delay(500);
  
}



