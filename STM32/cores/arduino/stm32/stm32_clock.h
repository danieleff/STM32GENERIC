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
    HAL_Delay(millis);
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

  while(start + microseconds > micros());
}

#ifdef __cplusplus
}
#endif

#endif
