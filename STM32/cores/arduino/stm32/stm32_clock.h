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

#ifdef __cplusplus
}
#endif

#endif
