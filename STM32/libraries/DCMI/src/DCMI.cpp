#include "DCMI.h"

uint16_t DCMI_FrameXferCplt;
uint16_t DCMI_LineXferCplt;
uint16_t DCMI_LineCounter;

uint16_t DCMI_Has_FrameXferCplt(void) {
  return DCMI_FrameXferCplt;
}

/*
 * These must NOT be defined in DCMI.h otherwise it overrides the function of HAL and will
 * not call back here on interrupt
 */

void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  DCMI_FrameXferCplt = 1;
}

void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi)
{
  DCMI_LineXferCplt = 1;
  DCMI_LineCounter++;
}

