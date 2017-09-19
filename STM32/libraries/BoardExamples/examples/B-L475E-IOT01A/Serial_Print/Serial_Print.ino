// Print milliseconds to Serial monitor every second

void setup() {
  SerialUART1.stm32SetTX(PB6);
  SerialUART1.stm32SetRX(PB7);
  SerialUART1.begin(115200); 
}

void loop() {
  SerialUART1.println(millis()); 
  delay(1000); 
}
