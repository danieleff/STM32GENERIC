/*
 HAL ( Hardware Abstraction Layer )

 Example on how access the HAL within the STM32GENERIC Arduino framework

 example copied from
 April 2017 michael_l

 http://www.stm32duino.com/viewtopic.php?f=42&t=1966&start=90#p27155

*/
//#include "STM32_HAL.h" not needed, auto included
uint32_t prev = 0;
int seconds = 0;

static TIM_HandleTypeDef s_TimerInstance = 
{
  .Instance = TIM2
};

void setup() 
{
  Serial.begin(115200); // Serial is SerialUSB

  __TIM2_CLK_ENABLE();
  s_TimerInstance.Init.Prescaler = 42000 - 1; //APB1 TIM2 has 84MHZ,
  s_TimerInstance.Init.CounterMode = TIM_COUNTERMODE_UP;
  s_TimerInstance.Init.Period = 2000 - 1; // prescaler gives 2khz time base
  s_TimerInstance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  s_TimerInstance.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&s_TimerInstance);
  HAL_TIM_Base_Start(&s_TimerInstance);
}

void loop() 
{
  if (__HAL_TIM_GET_COUNTER(&s_TimerInstance) == 0) 
  {
    uint32_t now = micros();
    Serial.print("diff=");
    Serial.println(now - prev);
    Serial.println(now);
    prev = micros();

    Serial.print(seconds++);
    Serial.println(" seconds");
  }
}
