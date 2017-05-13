/*  
  blink all LEDs 
  
  Black407VE ( STM32F407VE )
  http://wiki.stm32duino.com/images/5/5c/STM32_F4VE_SCHEMATIC.PDF

  May 2017, ChrisMicro
*/

/*
#define LED0_BUILTIN PA6 // same as LED_BUILTIN
#define LED1_BUILTIN PA7
*/

void setup()
{
  pinMode(LED0_BUILTIN, OUTPUT);
  pinMode(LED1_BUILTIN, OUTPUT);
}

void loop()
{
    digitalWrite(LED0_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED0_BUILTIN, LOW);
    
    digitalWrite(LED1_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED1_BUILTIN, LOW); 
}
