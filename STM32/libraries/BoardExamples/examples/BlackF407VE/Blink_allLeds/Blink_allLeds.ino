/*  
  blink all LEDs 

  remark: both leds are acrive low which means writing LOW 
          turns the led on, writing HIGH turns the led off
  
  Black407VE ( STM32F407VE )
  http://wiki.stm32duino.com/images/5/5c/STM32_F4VE_SCHEMATIC.PDF

  May 2017, ChrisMicro
*/

/*
  #define LED0_BUILTIN PA6 // same as LED_BUILTIN, active low
  #define LED1_BUILTIN PA7 // active low
*/

void setup()
{
  pinMode(LED0_BUILTIN, OUTPUT);
  pinMode(LED1_BUILTIN, OUTPUT);
  
  digitalWrite(LED0_BUILTIN, HIGH); // led off, led is active low
  digitalWrite(LED1_BUILTIN, HIGH); // led off, led is active low
}

void loop()
{
    digitalWrite(LED0_BUILTIN, LOW); // led on
    delay(1000);
    digitalWrite(LED0_BUILTIN, HIGH);// led off
    
    digitalWrite(LED1_BUILTIN, LOW); // led on
    delay(1000);
    digitalWrite(LED1_BUILTIN, HIGH); // led off
}
