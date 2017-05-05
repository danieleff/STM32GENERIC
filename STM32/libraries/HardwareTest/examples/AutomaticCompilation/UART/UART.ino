//Automatic testing compilation of basic SerialUART API

void setup() {
  SerialUART1.begin(115200);
  SerialUART1.println("test");
}

void loop() {
}

