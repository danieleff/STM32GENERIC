// show user button state with blinking LEDs
// STM32F429 Discovery has one user button
//
// red LED:   button pressed
// grenn LED: button not pressed

/*
#define LED_BUILTIN PG13
#define LED_GREEN                   LED_BUILTIN
#define LED_RED                     PG14 
#define LED_RED_OTG_OVERCURRENT     PC5

//On-board user button
#define USER_BTN                    PA0 
*/

void setup()
{
  pinMode(USER_BTN,   INPUT); // mandatory for STM MCUs, on Arduino Uno working without
  pinMode(LED_GREEN,  OUTPUT);
  pinMode(LED_RED,    OUTPUT);
}

void loop()
{
  if  ( digitalRead( USER_BTN ) )digitalWrite(LED_RED  , HIGH);
  else                           digitalWrite(LED_GREEN, HIGH);
  
  delay(100);

  // leds off
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED  , LOW);
  
  delay(100);
}
