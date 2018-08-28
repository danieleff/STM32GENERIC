# STM32DAC Lib
Arduino library to support STM32 DAC.

## Class & instance

* **`STM32DAC<DAC_CHANNEL_x>`**: creat a instance.

**param** `DAC_CHANNEL_x` is `DAC_CHANNEL_1` or `DAC_CHANNEL_2`.

* This lib had predefined two instances: `PA4_DACOUT` and `PA5_DACOUT`.

## API

* **`void Init()`**: initialized the DAC.

* **`void write(uint16_t val)`**: DAC output val.

* **`uint16_t read(void)`**: read the last DAC output val.