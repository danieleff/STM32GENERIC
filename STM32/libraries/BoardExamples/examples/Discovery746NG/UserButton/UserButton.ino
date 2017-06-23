// show user button state with blinking LED
// STM32F746 Discovery has one user button 
// 
// How is the program working?:
// It blinks faster when the button is pressed
//
// June 2017, ChrisMicro

/* connections of the pins
#define LED_BUILTIN         PI1
#define USER_BTN            PI11 
*/

void setup()
{
  pinMode(USER_BTN,    INPUT); // mandatory for STM MCUs, on Arduino Uno working without
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  digitalWrite( LED_BUILTIN  , HIGH ); // led on
  
  if  ( digitalRead( USER_BTN ) ) delay(10); // blink faster if button is pressed
  else                            delay(100);
  
  digitalWrite( LED_BUILTIN, LOW ); // led off
  delay(100);
}
