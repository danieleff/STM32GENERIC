// Print milliseconds to Serial monitor every second

void setup() {
  SerialUART1.begin(115200); 
}

void loop() {
  SerialUART1.println(millis()); 
  delay(1000); 
}
