// show user button state with blinking LEDs
// B-L475E-IOT01A Discovery has one user button


/*
#define LED_BUILTIN                 PA5
#define LED2_BUILTIN                PB14
#define LED3_BUILTIN                PC9  //Note LED3 and LED4 are both driven by PC9, but in reverse logic

//On-board user button
#define USER_BUTTON                 PC13
*/

int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  pinMode(USER_BUTTON, INPUT_PULLUP); 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED2_BUILTIN, OUTPUT);
  pinMode(LED3_BUILTIN, OUTPUT);

}

void loop() {

   buttonState = digitalRead(USER_BUTTON);
  
  if (buttonState == 1) {
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LED2_BUILTIN, HIGH);
  digitalWrite(LED3_BUILTIN, HIGH);
  delay(100); 
  }

   if (buttonState == 0) {
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED2_BUILTIN, LOW);
  digitalWrite(LED3_BUILTIN, LOW);
  delay(100);
  }

}
