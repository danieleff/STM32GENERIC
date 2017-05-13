/**
  ******************************************************************************
  * @file           : usbd_desc.c
  * @version        : v1.0_Cube
  * @brief          : This file implements the USB Device descriptors
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  * 1. Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  * this list of conditions and the following disclaimer in the documentation
  * and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of its contributors
  * may be used to endorse or promote products derived from this software
  * without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_conf.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_DESC 
  * @brief USBD descriptors module
  * @{
  */ 

/** @defgroup USBD_DESC_Private_TypesDefinitions
  * @{
  */ 

typedef struct __attribute__((__packed__)) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bString[];
} usb_descriptor_string;

/**
  * @}
  */ 

/** @defgroup USBD_DESC_Private_Defines
  * @{
  */ 
#define USBD_VID                         1155
#define USBD_LANGID_STRING               1033
#define USBD_MANUFACTURER_STRING         u"STMicroelectronics"

#define USBD_CDC_PID_FS                  22336
#define USBD_CDC_PRODUCT_STRING_FS       u"STM32 Virtual ComPort"
#define USBD_CDC_SERIALNUMBER_STRING_FS  u"00000000001A"
#define USBD_CDC_CONFIGURATION_STRING_FS u"CDC Config"
#define USBD_CDC_INTERFACE_STRING_FS     u"CDC Interface"

#define USBD_MSC_PID_FS                  22314
#define USBD_MSC_PRODUCT_STRING_FS       u"STM32 Mass Storage"
#define USBD_MSC_SERIALNUMBER_STRING_FS  u"00000000001A"
#define USBD_MSC_CONFIGURATION_STRING_FS u"MSC Config"
#define USBD_MSC_INTERFACE_STRING_FS     u"MSC Interface"

static const usb_descriptor_string USB_MANUFACTURER_STRING_DESCRIPTOR = {
    .bLength = sizeof(USBD_MANUFACTURER_STRING),
    .bDescriptorType = USB_DESC_TYPE_STRING,
    .bString = USBD_MANUFACTURER_STRING
};

static const usb_descriptor_string USB_CDC_PRODUCT_STRING_DESCRIPTOR = {
    .bLength = sizeof(USBD_CDC_PRODUCT_STRING_FS),
    .bDescriptorType = USB_DESC_TYPE_STRING,
    .bString = USBD_CDC_PRODUCT_STRING_FS
};

static const usb_descriptor_string USB_CDC_SERIALNUMBER_STRING_DESCRIPTOR = {
    .bLength = sizeof(USBD_CDC_SERIALNUMBER_STRING_FS),
    .bDescriptorType = USB_DESC_TYPE_STRING,
    .bString = USBD_CDC_SERIALNUMBER_STRING_FS
};

static const usb_descriptor_string USB_CDC_CONFIGURATION_STRING_DESCRIPTOR = {
    .bLength = sizeof(USBD_CDC_CONFIGURATION_STRING_FS),
    .bDescriptorType = USB_DESC_TYPE_STRING,
    .bString = USBD_CDC_CONFIGURATION_STRING_FS
};

static const usb_descriptor_string USB_CDC_INTERFACE_STRING_DESCRIPTOR = {
    .bLength = sizeof(USBD_CDC_INTERFACE_STRING_FS),
    .bDescriptorType = USB_DESC_TYPE_STRING,
    .bString = USBD_CDC_INTERFACE_STRING_FS
};



static const usb_descriptor_string USB_MSC_PRODUCT_STRING_DESCRIPTOR = {
    .bLength = sizeof(USBD_MSC_PRODUCT_STRING_FS),
    .bDescriptorType = USB_DESC_TYPE_STRING,
    .bString = USBD_MSC_PRODUCT_STRING_FS
};

static const usb_descriptor_string USB_MSC_SERIALNUMBER_STRING_DESCRIPTOR = {
    .bLength = sizeof(USBD_MSC_SERIALNUMBER_STRING_FS),
    .bDescriptorType = USB_DESC_TYPE_STRING,
    .bString = USBD_MSC_SERIALNUMBER_STRING_FS
};

static const usb_descriptor_string USB_MSC_CONFIGURATION_STRING_DESCRIPTOR = {
    .bLength = sizeof(USBD_MSC_CONFIGURATION_STRING_FS),
    .bDescriptorType = USB_DESC_TYPE_STRING,
    .bString = USBD_MSC_CONFIGURATION_STRING_FS
};

static const usb_descriptor_string USB_MSC_INTERFACE_STRING_DESCRIPTOR = {
    .bLength = sizeof(USBD_MSC_INTERFACE_STRING_FS),
    .bDescriptorType = USB_DESC_TYPE_STRING,
    .bString = USBD_MSC_INTERFACE_STRING_FS
};

/* USER CODE BEGIN 0 */

/* USER CODE END 0*/
/**
  * @}
  */ 

/** @defgroup USBD_DESC_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBD_DESC_Private_Variables
  * @{
  */ 
#ifdef USB_SUPPORT_USER_STRING_DESC
uint8_t *     USBD_FS_USRStringDesc (USBD_SpeedTypeDef speed, uint8_t idx , uint16_t *length);  
#endif /* USB_SUPPORT_USER_STRING_DESC */  

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USBD_CDC_DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END =
  {
    0x12,                       /*bLength */
    USB_DESC_TYPE_DEVICE,       /*bDescriptorType*/
    0x00,                       /* bcdUSB */  
    0x02,
    0x02,                        /*bDeviceClass*/
    0x02,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    USB_MAX_EP0_SIZE,          /*bMaxPacketSize*/
    LOBYTE(USBD_VID),           /*idVendor*/
    HIBYTE(USBD_VID),           /*idVendor*/
    LOBYTE(USBD_CDC_PID_FS),           /*idVendor*/
    HIBYTE(USBD_CDC_PID_FS),           /*idVendor*/
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    USBD_IDX_MFC_STR,           /*Index of manufacturer  string*/
    USBD_IDX_PRODUCT_STR,       /*Index of product string*/
    USBD_IDX_SERIAL_STR,        /*Index of serial number string*/
    USBD_MAX_NUM_CONFIGURATION  /*bNumConfigurations*/
  } ; 
/* USB_DeviceDescriptor */

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif



__ALIGN_BEGIN uint8_t USBD_MSC_DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END =
  {
    0x12,                       /*bLength */
    USB_DESC_TYPE_DEVICE,       /*bDescriptorType*/
#if (USBD_LPM_ENABLED == 1)
    0x01,                       /*bcdUSB */ /* changed to USB version 2.01
                                               in order to support LPM L1 suspend
                                               resume test of USBCV3.0*/
#else
    0x00,                       /* bcdUSB */
#endif
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    USB_MAX_EP0_SIZE,          /*bMaxPacketSize*/
    LOBYTE(USBD_VID),           /*idVendor*/
    HIBYTE(USBD_VID),           /*idVendor*/
    LOBYTE(USBD_MSC_PID_FS),           /*idVendor*/
    HIBYTE(USBD_MSC_PID_FS),           /*idVendor*/
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    USBD_IDX_MFC_STR,           /*Index of manufacturer  string*/
    USBD_IDX_PRODUCT_STR,       /*Index of product string*/
    USBD_IDX_SERIAL_STR,        /*Index of serial number string*/
    USBD_MAX_NUM_CONFIGURATION  /*bNumConfigurations*/
  } ;


/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END =
{
     USB_LEN_LANGID_STR_DESC,         
     USB_DESC_TYPE_STRING,       
     LOBYTE(USBD_LANGID_STRING),
     HIBYTE(USBD_LANGID_STRING), 
};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
__ALIGN_BEGIN uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;
/**
  * @}
  */ 

/** @defgroup USBD_DESC_Private_FunctionPrototypes
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBD_DESC_Private_Functions
  * @{
  */ 

/**
* @brief  USBD_FS_DeviceDescriptor 
*         return the device descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t *  USBD_CDC_DeviceDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  *length = sizeof(USBD_CDC_DeviceDesc);
  return USBD_CDC_DeviceDesc;
}


uint8_t *  USBD_MSC_DeviceDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  *length = sizeof(USBD_MSC_DeviceDesc);
  return USBD_MSC_DeviceDesc;
}

/**
* @brief  USBD_FS_LangIDStrDescriptor 
*         return the LangID string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t *  USBD_LangIDStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  *length =  sizeof(USBD_LangIDDesc);  
  return USBD_LangIDDesc;
}

/**
* @brief  USBD_FS_ProductStrDescriptor 
*         return the product string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t *  USBD_CDC_ProductStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  *length = USB_CDC_PRODUCT_STRING_DESCRIPTOR.bLength;
  return (uint8_t*)&USB_CDC_PRODUCT_STRING_DESCRIPTOR;
}

/**
* @brief  USBD_FS_ManufacturerStrDescriptor 
*         return the manufacturer string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t *  USBD_ManufacturerStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  *length = USB_MANUFACTURER_STRING_DESCRIPTOR.bLength;
  return (uint8_t*)&USB_MANUFACTURER_STRING_DESCRIPTOR;
}

/**
* @brief  USBD_FS_SerialStrDescriptor 
*         return the serial number string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t *  USBD_CDC_SerialStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  *length = USB_CDC_SERIALNUMBER_STRING_DESCRIPTOR.bLength;
  return (uint8_t*)&USB_CDC_SERIALNUMBER_STRING_DESCRIPTOR;
}

/**
* @brief  USBD_FS_ConfigStrDescriptor 
*         return the configuration string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t *  USBD_CDC_ConfigStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  *length = USB_CDC_CONFIGURATION_STRING_DESCRIPTOR.bLength;
  return (uint8_t*)&USB_CDC_CONFIGURATION_STRING_DESCRIPTOR;
}

/**
* @brief  USBD_HS_InterfaceStrDescriptor 
*         return the interface string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t *  USBD_CDC_InterfaceStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  *length = USB_CDC_INTERFACE_STRING_DESCRIPTOR.bLength;
  return (uint8_t*)&USB_CDC_INTERFACE_STRING_DESCRIPTOR;
}


uint8_t *  USBD_MSC_ProductStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  *length = USB_MSC_PRODUCT_STRING_DESCRIPTOR.bLength;
  return (uint8_t*)&USB_MSC_PRODUCT_STRING_DESCRIPTOR;
}

uint8_t *  USBD_MSC_SerialStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  *length = USB_MSC_SERIALNUMBER_STRING_DESCRIPTOR.bLength;
  return (uint8_t*)&USB_MSC_SERIALNUMBER_STRING_DESCRIPTOR;
}

uint8_t *  USBD_MSC_ConfigStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  *length = USB_MSC_CONFIGURATION_STRING_DESCRIPTOR.bLength;
  return (uint8_t*)&USB_MSC_CONFIGURATION_STRING_DESCRIPTOR;
}


uint8_t *  USBD_MSC_InterfaceStrDescriptor( USBD_SpeedTypeDef speed , uint16_t *length)
{
  *length = USB_MSC_INTERFACE_STRING_DESCRIPTOR.bLength;
  return (uint8_t*)&USB_MSC_INTERFACE_STRING_DESCRIPTOR;
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

USBD_DescriptorsTypeDef CDC_Desc =
{
  USBD_CDC_DeviceDescriptor,
  USBD_LangIDStrDescriptor,
  USBD_ManufacturerStrDescriptor,
  USBD_CDC_ProductStrDescriptor,
  USBD_CDC_SerialStrDescriptor,
  USBD_CDC_ConfigStrDescriptor,
  USBD_CDC_InterfaceStrDescriptor,
};

USBD_DescriptorsTypeDef MSC_Desc =
{
  USBD_MSC_DeviceDescriptor,
  USBD_LangIDStrDescriptor,
  USBD_ManufacturerStrDescriptor,
  USBD_MSC_ProductStrDescriptor,
  USBD_MSC_SerialStrDescriptor,
  USBD_MSC_ConfigStrDescriptor,
  USBD_MSC_InterfaceStrDescriptor,
};

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
