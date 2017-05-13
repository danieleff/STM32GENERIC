/**
 * Allocate 8MB ram, and write to Serial the results
 */

void setup() {
    Serial.begin(115200);
}

uint32_t size = 8 * 1024 * 1024 - 8 /*malloc overhead*/;

void loop() {

    Serial.println("===========");
    Serial.println("Allocating 8MB buffer");

    uint8_t *data = (uint8_t*)malloc(size);
    if (data == NULL) {
        Serial.println("Malloc failed!");
        delay(2000);
        return;
    }

    Serial.print("Buffer address: ");
    Serial.println((uint32_t)data, HEX);

    uint32_t start = millis();

    uint32_t sum = 0;
    uint32_t sum_check = 0;

    for(int i=0; i<size; i++) {
        data[i] = i & 0xff;
        sum += i & 0xff;
    }

    Serial.print("Write time: ");
    Serial.print(millis() - start);
    Serial.println(" ms");
    start = millis();

    for(int i=0; i<size; i++) {
        sum_check += data[i];
    }

    Serial.print("Read time: ");
    Serial.print(millis() - start);
    Serial.println(" ms");

    Serial.print("Checksum should be:");
    Serial.println(sum);
    Serial.print("Checksum calculated:");
    Serial.println(sum_check);

    if (sum == sum_check) {
        Serial.print("SDRAM is working with buffer size: ");
        Serial.println(size);
    } else {
        Serial.println("SDRAM NOT WORKING !!!");
    }

    free(data);

    delay(5000);
}
