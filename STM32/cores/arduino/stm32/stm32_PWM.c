#include "stm32_gpio.h"

TIM_HandleTypeDef *handle;

static uint32_t counter;
static uint32_t period;

#define min(a,b) ((a)<(b)?(a):(b))

stm32_pwm_disable_callback_func stm32_pwm_disable_callback = NULL;

void (*pwm_callback_func)();

void pwm_callback();

typedef struct {
    GPIO_TypeDef *port;
    uint32_t pin_mask;
    uint16_t frequency;
    uint16_t duty_cycle;
} stm32_pwm_type;

static stm32_pwm_type pwm_config[sizeof(variant_pin_list) / sizeof(variant_pin_list[0])];

void stm32_pwm_disable(GPIO_TypeDef *port, uint32_t pin);

void analogWrite(uint8_t pin, int value) {
    static TIM_HandleTypeDef staticHandle;

    if (handle == NULL) {
        handle = &staticHandle;
        pwm_callback_func = &pwm_callback;

        stm32_pwm_disable_callback = &stm32_pwm_disable;


        #ifdef TIM2 //99% of chips have TIM2
            __HAL_RCC_TIM2_CLK_ENABLE();
            HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(TIM2_IRQn);

            handle->Instance = TIM2;
        #else
            __HAL_RCC_TIM3_CLK_ENABLE();
            HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(TIM3_IRQn);

            handle->Instance = TIM3;
        #endif

        handle->Init.Prescaler = 999;
        handle->Init.CounterMode = TIM_COUNTERMODE_UP;
        period = 256;
        handle->Init.Period = period;
        handle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        HAL_TIM_Base_Init(handle);

        HAL_TIM_Base_Start_IT(handle);
    }

    for(int i=0; i<sizeof(pwm_config) / sizeof(pwm_config[0]); i++) {
        if (pwm_config[i].port == NULL ||
                (pwm_config[i].port == variant_pin_list[pin].port
                && pwm_config[i].pin_mask == variant_pin_list[pin].pin_mask)) {

            if (pwm_config[i].port == NULL) {
                pinMode(pin, OUTPUT);
            }

            pwm_config[i].port = variant_pin_list[pin].port;
            pwm_config[i].pin_mask = variant_pin_list[pin].pin_mask;
            pwm_config[i].frequency = 255;
            pwm_config[i].duty_cycle = value;
            break;
        }
    }
}

void stm32_pwm_disable(GPIO_TypeDef *port, uint32_t pin_mask) {
    for(int i=0; i<sizeof(pwm_config) / sizeof(pwm_config[0]); i++) {
        if (pwm_config[i].port == NULL) {
            return;
        }

        if (pwm_config[i].port == port && pwm_config[i].pin_mask == pin_mask) {

            for(int j = i + 1; j < sizeof(pwm_config) / sizeof(pwm_config[0]); j++) {
                if (pwm_config[j].port == NULL) {
                    pwm_config[i].port = pwm_config[j - 1].port;
                    pwm_config[i].pin_mask = pwm_config[j - 1].pin_mask;

                    pwm_config[j - 1].port = NULL;
                    break;
                }
            }

            break;
        }
    }
}

void pwm_callback() {
    counter += period;
    period = 256;

    for(int i=0; i<sizeof(pwm_config); i++) {
        if (pwm_config[i].port != NULL) {
            if (pwm_config[i].duty_cycle > counter % pwm_config[i].frequency) {
                pwm_config[i].port->BSRR = pwm_config[i].pin_mask;
                period = min(period, pwm_config[i].duty_cycle - (counter % pwm_config[i].frequency));
            } else {
                pwm_config[i].port->BSRR = pwm_config[i].pin_mask << 16;
                period = min(period, 256 - counter % pwm_config[i].frequency);
            }
        } else {
            break;
        }
    }

    if (!period) {
        period = 256;
    }
    __HAL_TIM_SET_AUTORELOAD(handle, period);
}

#ifdef TIM2
extern void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(handle);
}
#else
extern void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(handle);
}
#endif


extern void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (pwm_callback_func != NULL) {
        (*pwm_callback_func)();
    }
}
