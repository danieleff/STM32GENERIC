#include "Arduino.h"

#ifndef _STM32DAC_H_
#define _STM32DAC_H_

#if defined(DAC)|| defined(DAC1)

#ifdef __cplusplus
extern "C" {
#endif

void DAC_Init(uint32_t channel);
void DAC_SetValue(uint32_t channel, uint16_t value);
uint16_t DAC_GetValue(uint32_t channel);

#ifdef __cplusplus
}

template<uint32_t channel>
class STM32DAC {
  public:

    STM32DAC() {}

    void Init(void) {
      DAC_Init(channel);
    }

    void write(uint16_t value) {
      DAC_SetValue(channel, value);
    }

    inline uint16_t read(void){
	  	return DAC_GetValue(channel);
	}
	
    STM32DAC &operator = (uint16_t value) {
      write(value);
      return *this;
    }

    virtual ~STM32DAC() {}  // Do nothing
};

extern STM32DAC<DAC_CHANNEL_1>PA4_DACOUT;

#if defined(DAC_CHANNEL_2)
extern STM32DAC<DAC_CHANNEL_2>PA5_DACOUT;
#endif

#endif

#else
#error "This mcu does not support DAC!"
#endif

#endif /*_STM32DAC_H_*/
