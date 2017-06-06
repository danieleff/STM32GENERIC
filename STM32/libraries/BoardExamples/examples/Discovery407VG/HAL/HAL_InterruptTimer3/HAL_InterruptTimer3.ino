/*
 HAL ( Hardware Abstraction Layer )

 Timer 3 used to call an interrupt routine with constant repetion time.

 Example on how access the HAL within the STM32GENERIC Arduino framework.

 Hardware:
   STM32F4 Discovery

 contributers: michael_l, danieleff, ChrisMicro
 May 2017 

*/

static TIM_HandleTypeDef s_TimerInstance = 
{
  .Instance = TIM3
};

extern "C" void TIM3_IRQHandler(void) 
{
  HAL_TIM_IRQHandler(&s_TimerInstance);
  digitalWrite( LED_BUILTIN, !digitalRead(LED_BUILTIN) );
}

void setup() 
{
  pinMode(LED_BUILTIN,OUTPUT);

  HAL_NVIC_SetPriority( TIM3_IRQn, 0, 0 );
  HAL_NVIC_EnableIRQ  ( TIM3_IRQn );
  
  s_TimerInstance.Init.Prescaler = 42000 - 1; //APB1 TIM3 has 84MHZ,
  s_TimerInstance.Init.CounterMode = TIM_COUNTERMODE_UP;
  s_TimerInstance.Init.Period = 2000 - 1; // prescaler gives 2khz time base
  s_TimerInstance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  s_TimerInstance.Init.RepetitionCounter = 0;

  __HAL_RCC_TIM3_CLK_ENABLE();
  
  HAL_TIM_Base_Init     ( &s_TimerInstance );
  HAL_TIM_Base_Start_IT ( &s_TimerInstance );
}

void loop() 
{

}
