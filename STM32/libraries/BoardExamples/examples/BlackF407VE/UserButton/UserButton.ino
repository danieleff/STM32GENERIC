/*
  show user button state with LEDs
  
  Black407VE has 3 user buttons and 2 LEDs

  hardware:
    Black407VE ( STM32F407VE )
    http://wiki.stm32duino.com/images/5/5c/STM32_F4VE_SCHEMATIC.PDF

  May 2017, ChrisMicro
*/

/*
  #define USER_BUTTON0 PA0 // wakeup, active high
  #define USER_BUTTON1 PE3 // active low
  #define USER_BUTTON2 PE4 // active low

  
  #define LED0_BUILTIN PA6 // same as LED_BUILTIN, active low
  #define LED1_BUILTIN PA7 // active low
*/

void setup()
{
  pinMode(LED0_BUILTIN, OUTPUT);
  pinMode(LED1_BUILTIN, OUTPUT);

  pinMode(USER_BUTTON0, INPUT_PULLDOWN);
  pinMode(USER_BUTTON1, INPUT_PULLUP);
  pinMode(USER_BUTTON2, INPUT_PULLUP);  
}

void loop()
{
  // leds are active low
  digitalWrite(LED0_BUILTIN, LOW); // led on
  digitalWrite(LED1_BUILTIN, LOW); // led on


  // USER_BUTTON0 is active high
  if  ( digitalRead( USER_BUTTON0 ) != 0 ) digitalWrite(LED0_BUILTIN, HIGH); // led off

  // USER_BUTTON0 and USER_BUTTON1 are active low
  if  ( digitalRead( USER_BUTTON2 ) == 0 ) digitalWrite(LED1_BUILTIN, HIGH); // led off
  if  ( digitalRead( USER_BUTTON1 ) == 0 )
  {
    digitalWrite(LED0_BUILTIN, HIGH); // led off
    digitalWrite(LED1_BUILTIN, HIGH); // led off

  }
  
  delay(100);
}
