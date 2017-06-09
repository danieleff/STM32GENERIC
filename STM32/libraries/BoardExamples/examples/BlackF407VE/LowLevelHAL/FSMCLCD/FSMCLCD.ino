/**
 * FSMC on the J1 Header on Black F407VE board
 * LCD-style, D0..D15 + A18
 * http://wiki.stm32duino.com/images/5/5c/STM32_F4VE_SCHEMATIC.PDF
 *
 * Connect a logic analizer to the J1 header pins (check above schematic)
 */

void setup() {

    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_FSMC_CLK_ENABLE();

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                            |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                            |GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13
                            |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1
                            |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


    FSMC_NORSRAM_TimingTypeDef Timing;

    /** Perform the SRAM1 memory initialization sequence
    */
    SRAM_HandleTypeDef hsram1;
    hsram1.Instance = FSMC_NORSRAM_DEVICE;
    hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    /* hsram1.Init */
    hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
    hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
    hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
    hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
    hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
    hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
    hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
    hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
    hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
    hsram1.Init.PageSize = FSMC_PAGE_SIZE_NONE;

    /* Timing */
    Timing.AddressSetupTime = 0;
    Timing.AddressHoldTime = 1;
    Timing.DataSetupTime = 1;
    Timing.BusTurnAroundDuration = 15;
    Timing.CLKDivision = 16;
    Timing.DataLatency = 17;
    Timing.AccessMode = FSMC_ACCESS_MODE_A;

    if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK) {
        while(1);
    }

    pinMode(LED_BUILTIN, OUTPUT);
}

#define LCD_REG      (*((volatile unsigned short *) 0x60000000))

#define LCD_RAM      (*((volatile unsigned short *) 0x60000000 + (1 << 18)))

void loop() {
    digitalWrite(LED_BUILTIN, LOW);
    for(int32_t i=0; i<=UINT16_MAX; i++) {
        LCD_REG = i;
    }
    digitalWrite(LED_BUILTIN, HIGH);


    delay(200);


    digitalWrite(LED_BUILTIN, LOW);
    for(int32_t i=0; i<=UINT16_MAX; i++) {
        LCD_RAM = i;
    }
    digitalWrite(LED_BUILTIN, HIGH);


    delay(200);
}
