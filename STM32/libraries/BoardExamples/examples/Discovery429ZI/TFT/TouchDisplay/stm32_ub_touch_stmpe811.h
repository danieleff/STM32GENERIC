// from http://mikrocontroller.bplaced.net/wordpress/?page_id=3290
//
// modified to be used with STM32GENERIC Arduino framework
// Sept. 2017

//--------------------------------------------------------------
// File     : stm32_ub_touch_stmpe811.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_TOUCH_STMPE811_H
#define __STM32F4_UB_TOUCH_STMPE811_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
//#include "stm32f4xx.h"
//#include "stm32_ub_i2c3.h"
#include <stdint.h>
#include "Arduino.h"

uint8_t UB_I2C3_ReadByte(uint8_t addressI2cDevice, uint8_t registerId);
int16_t UB_I2C3_WriteByte(uint8_t addressI2cDevice, uint8_t adr, uint8_t wert);

// Touch-Status
typedef enum {
  TOUCH_PRESSED  = 0,   // Touch ist betaetigt
  TOUCH_RELEASED = 1	// Touch ist nicht betaetigt
}Touch_Status_t;


//-----------------------------------------
// Globale Struktur der Touch-Daten
//-----------------------------------------
typedef struct {
  Touch_Status_t status;
  uint16_t xp;
  uint16_t yp;
}Touch_Data_t;
extern Touch_Data_t Touch_Data;



#define IOE_ADC_FCT                0x01
#define IOE_TP_FCT                 0x02
#define IOE_IO_FCT                 0x04


//--------------------------------------------------------------
// Defines vom STMPE811
//--------------------------------------------------------------
#define STMPE811_ID                0x0811
#define STMPE811_I2C_ADDR          0x82
#define STMPE811_DELAY             200000

//--------------------------------------------------------------
// Register vom STMPE811
//--------------------------------------------------------------
#define IOE_REG_SYS_CTRL1          0x03
#define IOE_REG_SYS_CTRL2          0x04
#define IOE_REG_SPI_CFG            0x08

#define IOE_REG_INT_CTRL           0x09
#define IOE_REG_INT_EN             0x0A
#define IOE_REG_INT_STA            0x0B
#define IOE_REG_GPIO_INT_EN        0x0C
#define IOE_REG_GPIO_INT_STA       0x0D

#define IOE_REG_GPIO_SET_PIN       0x10
#define IOE_REG_GPIO_CLR_PIN       0x11
#define IOE_REG_GPIO_MP_STA        0x12
#define IOE_REG_GPIO_DIR           0x13
#define IOE_REG_GPIO_ED            0x14
#define IOE_REG_GPIO_RE            0x15
#define IOE_REG_GPIO_FE            0x16
#define IOE_REG_GPIO_AF            0x17


#define IOE_REG_ADC_INT_EN         0x0E
#define IOE_REG_ADC_INT_STA        0x0F
#define IOE_REG_ADC_CTRL1          0x20
#define IOE_REG_ADC_CTRL2          0x21
#define IOE_REG_ADC_CAPT           0x22
#define IOE_REG_ADC_DATA_CH0       0x30
#define IOE_REG_ADC_DATA_CH1       0x32
#define IOE_REG_ADC_DATA_CH2       0x34
#define IOE_REG_ADC_DATA_CH3       0x36
#define IOE_REG_ADC_DATA_CH4       0x38
#define IOE_REG_ADC_DATA_CH5       0x3A
#define IOE_REG_ADC_DATA_CH6       0x3B
#define IOE_REG_ADC_DATA_CH7       0x3C

#define IOE_REG_TP_CTRL            0x40
#define IOE_REG_TP_CFG             0x41
#define IOE_REG_WDM_TR_X           0x42
#define IOE_REG_WDM_TR_Y           0x44
#define IOE_REG_WDM_BL_X           0x46
#define IOE_REG_WDM_BL_Y           0x48
#define IOE_REG_FIFO_TH            0x4A
#define IOE_REG_FIFO_STA           0x4B
#define IOE_REG_FIFO_SIZE          0x4C
#define IOE_REG_TP_DATA_X          0x4D
#define IOE_REG_TP_DATA_Y          0x4F
#define IOE_REG_TP_DATA_Z          0x51
#define IOE_REG_TP_DATA_XYZ        0x52
#define IOE_REG_TP_FRACT_XYZ       0x56
#define IOE_REG_TP_DATA            0x57
#define IOE_REG_TP_I_DRIVE         0x58
#define IOE_REG_TP_SHIELD          0x59

//--------------------------------------------------------------
// sonstige
//--------------------------------------------------------------

#define IO_Pin_0                   0x01
#define IO_Pin_1                   0x02
#define IO_Pin_2                   0x04
#define IO_Pin_3                   0x08
#define IO_Pin_4                   0x10
#define IO_Pin_5                   0x20
#define IO_Pin_6                   0x40
#define IO_Pin_7                   0x80
#define IO_Pin_ALL                 0xFF



//--------------------------------------------------------------
// Defines vom Touch
//--------------------------------------------------------------
#define TOUCH_YD                   IO_Pin_1
#define TOUCH_XD                   IO_Pin_2
#define TOUCH_YU                   IO_Pin_3
#define TOUCH_XU                   IO_Pin_4
#define TOUCH_IO_ALL               (uint32_t)(IO_Pin_1 | IO_Pin_2 | IO_Pin_3 | IO_Pin_4)


//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
ErrorStatus UB_Touch_Init(void);
ErrorStatus UB_Touch_Read(void);


//--------------------------------------------------------------
#endif // __STM32F4_UB_TOUCH_STMPE811_H
