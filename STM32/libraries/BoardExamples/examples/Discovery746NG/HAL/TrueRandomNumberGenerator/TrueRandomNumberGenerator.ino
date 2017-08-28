/*
  TRNG ( true random number generator example )

  Some of the STM32 MCUs have a true random number generator.
 
  Hardware tested: STM32F746 Discovery

  This example code is in the public domain.

  2017 ChrisMicro
*/

RNG_HandleTypeDef RandomNumberGenerator_handle = {0};
  
void setup() 
{
  Serial.begin(115200);

  // enable random number generator clock
  __HAL_RCC_RNG_CLK_ENABLE(); 
  
  RandomNumberGenerator_handle.Instance = RNG;
  HAL_RNG_Init( &RandomNumberGenerator_handle );
}


void loop() 
{
  uint32_t randomValue32bit = HAL_RNG_GetRandomNumber( &RandomNumberGenerator_handle );

  // map the 32 bit random values into a range from 0..100
  int randomValue=map( randomValue32bit & 0xFFFF,0,0xFFFF,0,100);
  
  Serial.println( randomValue );
  
  delay(100);       
}
