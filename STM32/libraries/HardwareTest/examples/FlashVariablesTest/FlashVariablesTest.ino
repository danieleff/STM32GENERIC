/* Testing of writing random variables to Flash like EEPROM
*
* It uses RAM just for the test, and to see how the underlying storage changes when writing.
*/

#include "FlashVariables.h"

RamFlashPage page0(64);
RamFlashPage page1(64);

// number of variables that can be stored
#define VIRTUAL_STORAGE_LENGTH 4
// 1 byte data
#define DATA_BYTES 1

FlashVariables<DATA_BYTES> vars(&page0, &page1);

void setup() {
    Serial.begin(115200);

    delay(1000);
    Serial.println("Press a key to test");
}

uint8_t check[VIRTUAL_STORAGE_LENGTH];

void loop() {

    while(Serial.read() == -1);

    uint8_t data;

    int address = random(VIRTUAL_STORAGE_LENGTH);
    data = random(256);

    Serial.printf("Set variable %d= %d\n", address, data);

    check[address] = data;

    if (!vars.write(address, &data)) {
        Serial.printf("Failed to write");
        while(1);
    }

    Serial.print("Page 0 content: ");
    page0.print(Serial, 4);

    Serial.print("Page 1 content: ");
    page1.print(Serial, 4);
    for(int i=0; i<VIRTUAL_STORAGE_LENGTH; i++) {
        vars.read(i, &data);
        Serial.printf("%d=%d ", i, data);

        if (data != check[i]) {

            Serial.printf("ERROR %d, %d != %d", i, check[i], data);
            while(1);
        }
    }

    printf("\n");
}
