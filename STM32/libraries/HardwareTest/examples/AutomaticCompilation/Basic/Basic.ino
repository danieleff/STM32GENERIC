//Automatic testing compilation of basic Arduino API

void setup() {
  millis();
  micros();
  delay(1);
  delayMicroseconds(1);
  
  pinMode(0, OUTPUT);
  
  digitalWrite(0, HIGH);
  
  analogRead(0);
  
  analogWrite(0, 128);
  
  digitalWrite(0, 128);
}

void loop() {
}

