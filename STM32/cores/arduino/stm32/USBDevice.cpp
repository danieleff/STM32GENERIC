/*
  Copyright (c) 2017 Daniel Fekete

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "Arduino.h"

#if defined(USB_BASE) || defined(USB_OTG_DEVICE_BASE)

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
      for(i=0;i<1512;i++){};
      pinMode(PA12, INPUT);
      //digitalWrite(PA12, HIGH);
      //HAL_Delay(1000);
      for(i=0;i<512;i++){};
    #endif
}

bool USBDeviceClass::beginCDC() {
    reenumerate();

    USBD_Init(&hUsbDeviceFS, &CDC_Desc, DEVICE_FS);

    USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);

    USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);

    USBD_Start(&hUsbDeviceFS);

    return true;
}

bool USBDeviceClass::beginMSC() {
    reenumerate();

    USBD_Init(&hUsbDeviceFS, &MSC_Desc, DEVICE_FS);

    USBD_RegisterClass(&hUsbDeviceFS, &USBD_MSC);

    USBD_MSC_RegisterStorage(&hUsbDeviceFS, &USBD_DISK_fops);

    USBD_Start(&hUsbDeviceFS);

    return true;
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

USBD_HandleTypeDef hUsbDeviceFS;

USBDeviceClass USBDeviceFS;

#endif
