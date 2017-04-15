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
    return (HAL_GetTick()*1000) + ((SysTick->LOAD - SysTick->VAL) * 1000 / SysTick->LOAD);
}

inline void delayMicroseconds(uint32_t microseconds){
  uint32_t start = micros();

  while(start + microseconds > micros());
}

#ifdef __cplusplus
}
#endif

#endif
