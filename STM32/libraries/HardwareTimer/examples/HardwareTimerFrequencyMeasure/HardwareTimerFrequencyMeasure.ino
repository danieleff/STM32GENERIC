/*
* This example measures input frequency
*
* Select your board in https://danieleff.github.io/STM32GENERIC/ "STM32 Board" menu
*
* Check timer for which pin is the default TIM1/CH1 connected
*
* Check your board pinout which pin it is.
*
*
* Connect the testPwmPin to the previous input capture pin
*
* On Serial, you should see default PWM frequency
*/

#include "HardwareTimer.h"

const int testPwmOutputPin = PA0;

void setup() {
    analogWrite(testPwmOutputPin, 128);

    Serial.begin(115200);

    Timer1.setPrescaleFactor(100);
    Timer1.setOverflow(UINT16_MAX);

    Timer1.setMode(1, TIMER_INPUT_CAPTURE_RISING);
    Timer1.resume();
}

void loop() {

    uint32_t start = Timer1.getCompare(1);
    uint32_t end;
    do {
        //Wait for next rising edge
        end = Timer1.getCompare(1);
    } while(end == start);

    uint32_t diff;
    if (end > start) {
        diff = end - start;
    } else {
        diff = (Timer1.getOverflow() - start) + end;
    }
    uint32_t timerFrequency = Timer1.getBaseFrequency() / (Timer1.getPrescaleFactor() + 1);

    Serial.print("Input frequency: ");
    Serial.print((float)timerFrequency / diff, 6);
    Serial.println(" Hz");


    delay(1000);
}
