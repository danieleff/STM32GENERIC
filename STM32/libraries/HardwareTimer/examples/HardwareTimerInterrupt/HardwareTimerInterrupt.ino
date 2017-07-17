/*
* This example sets timer1 software interrupt
*/

#include "HardwareTimer.h"

void interrupt() {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void setup() {
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);

    Timer1.setPeriod(1000000); // in microseconds = 1 second

    Timer1.attachInterrupt(interrupt);
    Timer1.resume();
}

void loop() {
    delay(5000);

    Serial.println("----");
    Serial.print("Timer base frequency:");
    Serial.println(Timer1.getBaseFrequency());

    Serial.print("Timer prescaler:");
    Serial.println(Timer1.getPrescaleFactor());

    Serial.print("Timer period:");
    Serial.println(Timer1.getOverflow());
}
