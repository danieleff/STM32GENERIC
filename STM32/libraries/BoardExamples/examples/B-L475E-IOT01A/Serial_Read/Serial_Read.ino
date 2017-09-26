/* Reads incoming Serial data and responds accordingly. 
   Send "L" to turn LED off
   Send "H" to turn LED on   */


int incomingByte;      // variable to save incoming serial data 

void setup() {
  // initialize SerialUART1 communication:
  SerialUART1.begin(115200);
  
  // initialize the LED pin as an output:
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // see if there's incoming SerialUART1 data:
  if (SerialUART1.available() > 0) {

    // read the oldest byte in the SerialUART1 buffer:

    incomingByte = SerialUART1.read();

    // if it's a capital H, turn on the LED:
    if (incomingByte == 'H') {
      digitalWrite(LED_BUILTIN, HIGH);
      SerialUART1.println("HIGH ");
     } 

    // if it's an L, turn off the LED:
    else if (incomingByte == 'L') {
      digitalWrite(LED_BUILTIN, LOW);
      SerialUART1.println("LOW ");
    }
  }
}
