#ifndef __FSMCTFT_H__
#define __FSMCTFT_H__

#include <Arduino.h>

class FSMCTFT {
 public:
  SRAM_HandleTypeDef hsram1;
  FSMCTFT(void);
  void begin(void);
  void end(void);
};

#endif /*__FSMC_H__ */
