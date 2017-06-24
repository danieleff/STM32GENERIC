#include "stm32_build_defines.h"
#include "stm32_def.h"
void Error_Handler(void);

void SystemClock_Config(void){
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;
  /**Initializes the CPU, AHB and APB busses clocks  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
 if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) { Error_Handler(); }
 PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_USART3 |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_TIM1 |RCC_PERIPHCLK_TIM1|RCC_PERIPHCLK_ADC12 |RCC_PERIPHCLK_ADC34; PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2; PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1; PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1; PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1; PeriphClkInit.Adc34ClockSelection = RCC_ADC34PLLCLK_DIV1; PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI; PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5; PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK; if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) { Error_Handler(); }
 /**Configure the Systick interrupt time  */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
 /**Configure the Systick  */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
 /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);}/* USER CODE END 2 */
  //void SystemClock_Config(void)
  //{//#if defined(RUN_HSI)
  // SystemClock_Config_HSI();//#else// SystemClock_Config_HSE();//#endif //}
/** * @} * @} */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
