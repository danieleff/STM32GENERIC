#include "Arduino.h"

#include "USBDevice.h"
#include "usbd_core.h"
#include "usbd_desc.h"

#include "cdc/usbd_cdc.h"
#include "cdc/usbd_cdc_if.h"

#include "msc/usbd_msc.h"

#include "usb_device.h"

void USBDeviceClass::reenumerate() {
    /* Re-enumerate the USB */
      volatile unsigned int i;

    #ifdef USB_DISC_PIN
      pinMode(USB_DISC_PIN, OUTPUT);
      digitalWrite(USB_DISC_PIN, HIGH);
        for(i=0;i<512;i++);
      digitalWrite(USB_DISC_PIN, LOW);
    #else
      //pinMode(USBDP_PIN, OUTPUT);
      //digitalWrite(USBDP_PIN, LOW);
        //for(i=0;i<512;i++);
      //digitalWrite(USBDP_PIN, HIGH);


      pinMode(PA12, OUTPUT);
      digitalWrite(PA12, LOW);
      //HAL_Delay(1000);
      for(i=0;i<512;i++){};
      digitalWrite(PA12, HIGH);
      //HAL_Delay(1000);
      for(i=0;i<512;i++){};
    #endif
}

bool USBDeviceClass::beginMenuSelection() {

#ifdef MENU_USB_SERIAL
    return beginCDC();
#elif MENU_USB_MASS_STORAGE
    return  beginMSC();
#else
    return false;
#endif

}

bool USBDeviceClass::beginCDC() {
    reenumerate();

    USBD_Init(&hUsbDeviceFS, &CDC_Desc, DEVICE_FS);

    USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);

    USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);

    USBD_Start(&hUsbDeviceFS);
}

namespace Testing {
extern USBD_StorageTypeDef USBD_DISK_fops;
}

bool USBDeviceClass::beginMSC() {
    reenumerate();

    USBD_Init(&hUsbDeviceFS, &MSC_Desc, DEVICE_FS);

    USBD_RegisterClass(&hUsbDeviceFS, &USBD_MSC);

    USBD_MSC_RegisterStorage(&hUsbDeviceFS, &Testing::USBD_DISK_fops);

    USBD_Start(&hUsbDeviceFS);
}

extern PCD_HandleTypeDef hpcd_USB_FS;
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

//F1
extern "C" void USB_LP_CAN1_RX0_IRQHandler(void) {
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
}
//F4 F7
extern "C" void OTG_FS_IRQHandler(void) {
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}
//L0
extern "C" void USB_IRQHandler(void) {
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
}

extern "C" void USBSerial_Rx_Handler(uint8_t *data, uint16_t len){
  SerialUSB.CDC_RxHandler(data, len);
}

USBD_HandleTypeDef hUsbDeviceFS;

USBDeviceClass USBDeviceFS;
