/*
  Copyright (c) 2017 Daniel Fekete

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

/**
* Implementation of Arduino methods:
* millis() https://www.arduino.cc/en/Reference/Millis
* micros() https://www.arduino.cc/en/Reference/Micros
* delay() https://www.arduino.cc/en/Reference/Delay
* TODO delayMicroseconds() https://www.arduino.cc/en/Reference/DelayMicroseconds
*/
#ifndef STM32_CLOCK_H
#define STM32_CLOCK_H

#ifdef __cplusplus
extern "C"{
#endif

inline void delay(unsigned long millis) {
    uint32_t tickstart = 0;
    tickstart = HAL_GetTick();
    while((HAL_GetTick() - tickstart) < millis) {
        yield();
    }
}

inline uint32_t millis() {
    return HAL_GetTick();
}

inline uint32_t micros() {
  // by Pito 4/2017
  uint32_t m = HAL_GetTick();
  uint32_t u = SysTick->LOAD - SysTick->VAL;
  uint32_t m1 = HAL_GetTick();
  uint32_t u1 = SysTick->LOAD - SysTick->VAL;

  if (m1 > m) {
    return ( m1 * 1000 + (u1 * 1000) / SysTick->LOAD);
  } else {
    return ( m * 1000 + (u * 1000) / SysTick->LOAD);
  }
}

inline void delayMicroseconds(uint32_t microseconds){
  uint32_t start = micros();

  while(start + microseconds > micros()) {
      yield();
  }
}

void stm32ScheduleMicros(uint32_t microseconds, void (*callback)());

#ifdef __cplusplus
}
#endif

#endif
