//Automatic testing compilation of blink sketch

#ifndef LED_BUILTIN
    #define LED_BUILTIN 0
#endif

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(1000);
}
