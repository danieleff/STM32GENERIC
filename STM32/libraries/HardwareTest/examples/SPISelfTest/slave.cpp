#include "SPI.h"

SPI_HandleTypeDef hspi2 = {};

volatile uint32_t slave_index = 0;
uint8_t slave_out[1024];
uint8_t slave_in[1024];

void slave() {
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  __HAL_RCC_SPI2_CLK_ENABLE();
  
  GPIO_InitTypeDef GPIO_InitStruct;
  
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(SPI2_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(SPI2_IRQn);
    
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_SLAVE;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_HARD_INPUT;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi2);
  
}

void slave_reset() {
  slave_index = 0;
  
  SPI2->DR = slave_out[slave_index];
  HAL_SPI_Receive_IT(&hspi2, slave_in + slave_index, 1);
}

extern "C" void SPI2_IRQHandler(void) {
  HAL_SPI_IRQHandler(&hspi2);
}

extern "C" void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
  slave_index++;
  SPI2->DR = slave_out[slave_index];
  HAL_SPI_Receive_IT(&hspi2, slave_in + slave_index, 1);
}
