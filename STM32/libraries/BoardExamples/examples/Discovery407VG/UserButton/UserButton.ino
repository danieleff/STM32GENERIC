// show user button state with blinking LEDs
// STM32F4 Discovery has one user button
//
// red LED:   button pressed
// grenn LED: button not pressed

#define USERBUTTON               PA0

#define GREENLED                PD12
#define ORANGELED               PD13
#define REDLED                  PD14
#define BLUELED                 PD15
#define REDLEDOTGOVERCURRENT     PD5

void setup()
{
  pinMode(USERBUTTON, INPUT); // mandatory for STM MCUs, on Arduino Uno working without
  pinMode(GREENLED,  OUTPUT);
  pinMode(REDLED,    OUTPUT);
}

void loop()
{
  if  ( digitalRead(USERBUTTON) )digitalWrite(REDLED  , HIGH);
  else                           digitalWrite(GREENLED, HIGH);
  
  delay(100);

  // leds off
  digitalWrite(GREENLED, LOW);
  digitalWrite(REDLED  , LOW);
  
  delay(100);
}
