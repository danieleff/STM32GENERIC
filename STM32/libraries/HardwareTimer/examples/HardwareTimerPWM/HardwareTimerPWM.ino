/*
* This example sets timer1 / channel1 to 10% PWM
*
* Select your board in https://danieleff.github.io/STM32GENERIC/ "STM32 Board" menu
*
* Check timer for which pin is the default TIM1/CH1 connected
*
* Check your board pinout which pin it is.
*
* Use scope / logic analyzer to check PWM output
*/

#include "HardwareTimer.h"

void setup() {
    Timer1.setPrescaleFactor(65535);
    Timer1.setOverflow(99); //Period

    Timer1.setCompare(1, 10); //Channel / Duty cycle
    Timer1.setMode(1, TIMER_PWM);

    //Timer1.setMode(1, TIMER_PWM, PA8);                   // Alternatively for non-default pin. Check chip documentation!
    //Timer1.setMode(1, TIMER_OUTPUT_COMPARE_TOGGLE);      // Pin toggles when counter reaches compare (effectively 50% PWM)

    Timer1.resume();
}

void loop() {

}
