#include <Arduino.h>

#include "Servo.h"

Servo::Servo() {
    this->pin = INVALID_PIN;
    this->min = 0;
    this->max = 180;
    this->microSeconds = 0;
}

uint8_t Servo::attach(int pin) {
    this->pin = pin;
    this->min = 0;
    this->max = 180;
    return 1;
}

uint8_t Servo::attach(int pin, int min, int max) {
    this->pin = pin;
    this->min = min;
    this->max = max;
    return 1;
}

void Servo::detach() {
    pinMode(this->pin, INPUT);
    this->pin = INVALID_PIN;
}

void Servo::write(int value) {
    if(value < 360) {  // treat values less than 360 as angles in degrees (valid values in microseconds are handled as microseconds)
        if(value < 0) value = 0;
        if(value > 180) value = 180;
        value = map(value, 0, 180, 1000, 2000);
    }
    this->writeMicroseconds(value);
}

void Servo::writeMicroseconds(int microSeconds) {
    this->microSeconds = microSeconds;

    long dutyCycle16Bits = map(microSeconds, 1000, 2000, (1 << 16) / (1000 / 50), (2 << 16) / (1000 / 50));

    pwmWrite(this->pin, dutyCycle16Bits, 50, 0);
}

int Servo::read() {
    return map( microSeconds, 1000, 2000, 0, 180);
}

int Servo::readMicroseconds()
{
  return microSeconds;
}

bool Servo::attached() {
  return this->pin != INVALID_PIN;
}
