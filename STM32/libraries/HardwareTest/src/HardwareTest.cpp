#include "HardwareTest.h"
#include <stdarg.h>

void TestGroup::begin() {
	fails = 0;
	totals = 0;
	Serial.println(name);
}

void TestGroup::assert(boolean success, const char *message, ...) {
    char buf[128];

    va_list argptr;
    va_start(argptr, message);
    vsnprintf(buf, sizeof(buf), message, argptr);
    va_end(argptr);

    totals++;
    test->totals++;
    if (success) {

    } else {
        fails++;
        test->fails++;
        Serial.print("!!! ");
        Serial.println(buf);
    }
}

void TestGroup::end() {
	Serial.print(": ");
	Serial.print(totals);
	Serial.print(" total, ");
	Serial.print(fails);
	Serial.println(" failed");

	test->setLed(false);
	delay(100);
	test->setLed(true);
	if (fails > 0) {
		delay(700);
	} else {
		delay(100);
	}
	test->setLed(false);
}

void HardwareTest::begin() {
	totals = 0;
	fails = 0;
	Serial.println(" --- ");
	Serial.println(" BEGIN TEST ");
}
void HardwareTest::end() {

    Serial.println(" --- ");
    Serial.print("TOTAL: ");
    Serial.println(totals);
    Serial.print("FAILS: ");
    Serial.println(fails);
    Serial.println(" --- END TESTING --- ");
}

void HardwareTest::setLedPin(uint16_t ledPin, boolean activeOnLow) {
    this->ledPin = ledPin;
    this->ledActiveLow = activeOnLow;
}

void HardwareTest::setLed(boolean on) {
	pinMode(ledPin, OUTPUT);

    digitalWrite(ledPin, (on != ledActiveLow) ? HIGH : LOW);
}

void HardwareTest::toggleLed() {
	pinMode(ledPin, OUTPUT);

    digitalWrite(ledPin, !digitalRead(ledPin));
}
