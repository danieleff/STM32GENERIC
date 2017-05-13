/*

 RAM free example.

 This example shows the free RAM which could be allocated by a program.
 When you compile a program RAM is used by variables and arrays.
 The remaining RAM can b e dynamically allocated during the run time of the 
 progam. Usually this is done with the function "malloc".
 
 This examples shows who much RAM is left for dynamic allocation.
 
 May 2017, ChrisMicro
 
 */

#define mySerialPort Serial          // equal to SerialUSB
//#define mySerialPort SerialUART1 
//#define mySerialPort SerialUART2

// Freestack is found here
// https://github.com/greiman/SdFat-beta/blob/master/SdFat/src/FreeStack.h#L45
// from William Greiman
#if defined(__arm__)
extern "C" char* sbrk(int incr);
static int FreeStack() {
  char top = 't';
  return &top - reinterpret_cast<char*>(sbrk(0));
}
#endif

void setup() 
{
  mySerialPort.begin(115200);
  delay(3000);
  mySerialPort.println("Ram Free Example");
  mySerialPort.println("show the RAM space available");
}

int Counter=0;

void loop() 
{
  uint32_t ram;
  
  ram = FreeStack();
  
  mySerialPort.print("free RAM: "); 
  mySerialPort.print(ram);  mySerialPort.print(" bytes   is   "); 
  mySerialPort.print(ram/1024);  mySerialPort.println(" KB");
  
  delay(1000);
}

