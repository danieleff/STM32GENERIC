#include "STM32DAC.h"

#ifdef STM32H7
#define DAC DAC1
#define __HAL_RCC_DAC1_CLK_ENABLE __HAL_RCC_DAC12_CLK_ENABLE
#endif

static DAC_HandleTypeDef DAC_Handle[2];
static DAC_ChannelConfTypeDef  DAC_ChannelConf;

extern "C"
void DAC_Init(uint32_t DACx) {
  if (DACx == DAC_CHANNEL_1) {
    pinMode(PA4, INPUT);
  } 
#if defined(DAC_CHANNEL_2)
  else {
    pinMode(PA5, INPUT);
  }
#endif

#if defined(__HAL_RCC_DAC1_CLK_ENABLE)
  __HAL_RCC_DAC1_CLK_ENABLE();
#else
  __HAL_RCC_DAC_CLK_ENABLE();
#endif

  DAC_Handle[(uint8_t)DACx].Instance = DAC;
  
  HAL_DAC_Init(&DAC_Handle[(uint8_t)DACx]);

#if 0
  DAC_ChannelConf.DAC_SampleAndHold =  DAC_SAMPLEANDHOLD_DISABLE;
  DAC_ChannelConf.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  DAC_ChannelConf.DAC_UserTrimming =  DAC_TRIMMING_FACTORY;
#endif
  
  DAC_ChannelConf.DAC_Trigger = DAC_TRIGGER_NONE;
  DAC_ChannelConf.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (DACx == DAC_CHANNEL_1) {
     HAL_DAC_ConfigChannel(&DAC_Handle[(uint8_t)DACx], &DAC_ChannelConf, DAC_CHANNEL_1);
     DAC->CR |= DAC_CR_EN1;
	 }
#if defined(DAC_CHANNEL_2)
  else {
    HAL_DAC_ConfigChannel(&DAC_Handle[(uint8_t)DACx], &DAC_ChannelConf, DAC_CHANNEL_2);
    DAC->CR |= DAC_CR_EN2;
  }
#endif  
}

extern "C"
void DAC_SetValue(uint32_t DACx, uint16_t value) {
  if (value > 0x0FFF) {
    value = 0x0FFF;
  }

  /* Set 12-bit value, right aligned */
  if (DACx == DAC_CHANNEL_1) {
    HAL_DAC_SetValue(&DAC_Handle[(uint8_t)DACx], DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
  }
#if defined(DAC_CHANNEL_2)
  else {
    HAL_DAC_SetValue(&DAC_Handle[(uint8_t)DACx], DAC_CHANNEL_2, DAC_ALIGN_12B_R, value);
  }
#endif  
}

extern "C"
uint16_t DAC_GetValue(uint32_t DACx) {
  if (DACx == DAC_CHANNEL_1) {
    return (uint16_t)HAL_DAC_GetValue(&DAC_Handle[(uint8_t)DACx],DAC_CHANNEL_1);
  }
#if defined(DAC_CHANNEL_2)
  else{
    return (uint16_t)HAL_DAC_GetValue(&DAC_Handle[(uint8_t)DACx],DAC_CHANNEL_2);
  }
#endif
	
}

STM32DAC<DAC_CHANNEL_1>dacChannel_1;

#if defined(DAC_CHANNEL_2)
STM32DAC<DAC_CHANNEL_2>dacChannel_2;
#endif
