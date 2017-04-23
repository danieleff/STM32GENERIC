// blink all LEDs 
// STM32F4 Discovery

#define GREENLED                PD12
#define ORANGELED               PD13
#define REDLED                  PD14
#define BLUELED                 PD15
#define REDLEDOTGOVERCURRENT     PD5


#define NUMLEDS 5
const uint8_t Led[] = {GREENLED, ORANGELED, REDLED, BLUELED, REDLEDOTGOVERCURRENT};

void setup()
{
  for (int n = 0; n < NUMLEDS; n++)  pinMode(Led[n], OUTPUT);
}

void setLed(uint8_t n, uint8_t state)
{
  if(n==4)digitalWrite(Led[n], !state);
  else digitalWrite(Led[n], state);
}

void loop()
{
  for (int n = 0; n < NUMLEDS; n++)
  {
    setLed(n, HIGH);
    delay(100);
    setLed(n, LOW);
    delay(100);
  }
}