// Blink on-board LEDs


/*
#define LED_BUILTIN                 PA5
#define LED2_BUILTIN                PB14
#define LED3_BUILTIN                PC9  //Note LED3 and LED4 are both driven by PC9, but in reverse logic

*/

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED2_BUILTIN, OUTPUT);
  pinMode(LED3_BUILTIN, OUTPUT);

}

void loop() {

  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LED2_BUILTIN, HIGH);
  digitalWrite(LED3_BUILTIN, HIGH);
  delay(400); 
   
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED2_BUILTIN, LOW);
  digitalWrite(LED3_BUILTIN, LOW);
  delay(300);
  
}
