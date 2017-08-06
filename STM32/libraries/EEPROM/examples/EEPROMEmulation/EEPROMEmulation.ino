/**
 * EEPROM emulation in internal flash example
 *
 * For more examples, go to https://www.arduino.cc/en/Reference/EEPROM
 */

#include "EEPROM.h"

void setup() {
    // If you want to format the flash storage:
    // EEPROM.format();
}

void loop() {
    delay(2000);

    int index = 2; // random index

    EEPROM.update(index, EEPROM.read(index) + 1);

    Serial.printf("EEPROM value at index %d is %d. Reset the board, and the value will persist.\n", index, EEPROM.read(index));
    Serial.printf("You can store up to %d bytes in EEPROM\n", EEPROM.length());
    Serial.println("It is recommended to use less than that (around half) to reduce wear leveling of the Flash");
    Serial.println();

}
