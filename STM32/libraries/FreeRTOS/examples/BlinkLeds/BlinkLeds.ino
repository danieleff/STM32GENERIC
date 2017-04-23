/**
 * Blink 2 leds using FreeRTOS
 * For documentation, check http://www.freertos.org/
 */

#include <Arduino_FreeRTOS.h>

uint8_t led1 = LED_BUILTIN;
uint8_t led2 = PA7;

void taskBlink1( void *pvParameters) {
    pinMode(led1, OUTPUT);

    while(1) {
        digitalWrite(led1, !digitalRead(led1));
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
}

void taskBlink2( void *pvParameters) {
    pinMode(led2, OUTPUT);

    while(1) {
        digitalWrite(led2, !digitalRead(led2));
        vTaskDelay( 500 / portTICK_PERIOD_MS );
    }
}

void setup() {

    xTaskCreate(taskBlink1,  "Blink 1",  128,  NULL,  1,  NULL );
    xTaskCreate(taskBlink2,  "Blink 2",  128,  NULL,  1,  NULL );

    vTaskStartScheduler();
}

void loop() {

}
