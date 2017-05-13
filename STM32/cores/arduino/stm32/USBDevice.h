#ifndef _SERIAL_USBDEVICE_H_INCLUDED
#define _SERIAL_USBDEVICE_H_INCLUDED

#include "stdint.h"

class USBDeviceClass {
  public:
    void reenumerate();

    bool beginMenuSelection();

    bool beginCDC();

    bool beginMSC();

};

extern USBDeviceClass USBDeviceFS;


#endif
