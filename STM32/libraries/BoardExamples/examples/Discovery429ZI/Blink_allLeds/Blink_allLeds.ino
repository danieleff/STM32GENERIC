// blink all LEDs 
// STM32F429 Discovery

#define GREENLED                PG13
#define REDLED                  PG14

#define NUMLEDS 2
const uint8_t Led[] = {GREENLED, REDLED};

void setup()
{
  for (int n = 0; n < NUMLEDS; n++)  pinMode(Led[n], OUTPUT);
  //pinMode(OTGFSOC_LED,PULL_UP); // needed to protect STMPS2141 FAULT output from shortcutting PC5 on power fault
}

void setLed(uint8_t n, uint8_t state)
{
  if(n==2)digitalWrite(Led[n], !state);
  else digitalWrite(Led[n], state);
}

void loop()
{
  for (int n = 0; n < NUMLEDS; n++)
  {
    setLed(n, HIGH);
    delay(500);
    setLed(n, LOW);
    delay(500);
  }
}
