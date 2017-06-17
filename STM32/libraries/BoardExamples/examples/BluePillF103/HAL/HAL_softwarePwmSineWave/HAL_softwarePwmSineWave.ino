/*
  Software Audio PWM

  Timer 3 used to create a software audo pwm.
  This function is useful when you want to create some audio sounds
  and you don't hava a DAC.

  With this fast PWM and a RC-Low pass filter you can produce with
  minimum efforts audio sounds.

  Restrictions and hints:
  - with this routine the intterrupt load is double of a hardware PEM impementation
  - the voltage after the RC-lowpas has only a range of ~2Vpp because of the interrupt
  processing time needs

  HAL ( Hardware Abstraction Layer )
  Example on how access the HAL within the STM32GENERIC Arduino framework.

  contributers and preliminara works by: michael_l, danieleff

  June 2015, ChrisMicro

*/

static TIM_HandleTypeDef s_TimerInstance =
{
  .Instance = TIM3
};

#define SOUNDPIN PB0

#define MEANPERIOD        500
#define DAC_MAX_AMPLITUDE 256
//#define SAMPLINGFREQUENCY 44100
#define WAVEBUFFERLENGTH  256

int32_t WaveBuffer[WAVEBUFFERLENGTH];

// this default values will be overwritten
volatile int32_t PeriodLow  = 1000;
volatile int32_t PeriodHigh = 1000;

void setAmplitude(int32_t amplitude)
{
  PeriodLow  = MEANPERIOD - amplitude;
  PeriodHigh = MEANPERIOD + amplitude;
}

extern "C" void TIM3_IRQHandler(void)
{
  static boolean flag = false;
  static int32_t index = 0;

  if (__HAL_TIM_GET_FLAG(&s_TimerInstance, TIM_FLAG_UPDATE) != RESET)
  {
    if (__HAL_TIM_GET_IT_SOURCE(&s_TimerInstance, TIM_IT_UPDATE) != RESET)
    {

      __HAL_TIM_CLEAR_IT(&s_TimerInstance, TIM_IT_UPDATE);

      digitalWrite( SOUNDPIN, flag );
      if (flag) __HAL_TIM_SET_AUTORELOAD(&s_TimerInstance, PeriodLow);
      else
      {
        __HAL_TIM_SET_AUTORELOAD( &s_TimerInstance, PeriodHigh );

        setAmplitude( WaveBuffer[ index++ ] );
        if ( index > WAVEBUFFERLENGTH ) index = 0 ;
      }
      flag = !flag;
    }
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SOUNDPIN, OUTPUT);

  HAL_NVIC_SetPriority( TIM3_IRQn, 0, 0 );
  HAL_NVIC_EnableIRQ  ( TIM3_IRQn );

  s_TimerInstance.Init.Prescaler = 0; //APB1 TIM3 has 84MHZ, 72MHz

  s_TimerInstance.Init.CounterMode = TIM_COUNTERMODE_UP;
  s_TimerInstance.Init.Period = 2000 - 1; // prescaler gives 2khz time base
  s_TimerInstance.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  s_TimerInstance.Init.RepetitionCounter = 0;

  __HAL_RCC_TIM3_CLK_ENABLE();

  HAL_TIM_Base_Init     ( &s_TimerInstance );
  HAL_TIM_Base_Start_IT ( &s_TimerInstance );

  // initialize sine wave buffer
  for (int n = 0; n < WAVEBUFFERLENGTH; n++)
  {
    const float   frequency = 440;
    const float   amplitude = DAC_MAX_AMPLITUDE;
    //int32_t val = ( sin( 2 * PI * frequency / SAMPLINGFREQUENCY * n )) * amplitude;
    int32_t val = ( sin( 2 * PI * n / WAVEBUFFERLENGTH  )) * amplitude;
    WaveBuffer[n] = val;
  }
}

void loop()
{
  // to noughting, sound is produced in interrupt routine
}
