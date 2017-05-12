## USB: Mass Storage

## **This library is in beta, and the API / implementation is for testing only!**

Until there is an official API, you can test mass storage with the following code:

```c++
#include "msc/usbd_msc.h"

const uint8_t MSC_Inquirydata[] = {
  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0' ,'1',                     /* Version      : 4 Bytes */
};

const uint16_t blockSize = 512;
const uint32_t blockCount = 100;

uint8_t ramdisk[blockSize * blockCount] = {0};

namespace Testing {

// See UM1734, 6.2.4
// See usbd_msc.h
USBD_StorageTypeDef USBD_DISK_fops = {
    //Init
    [](uint8_t lun) -> int8_t {
        return USBD_OK;
    },

    //GetCapacity
    [](uint8_t lun, uint32_t *block_num, uint16_t *block_size) -> int8_t {
        *block_num  = blockCount;
        *block_size = blockSize;
        return USBD_OK;
    },

    //IsReady
    [](uint8_t lun) -> int8_t {
        return USBD_OK;
    },

    //IsWriteProtected
    [](uint8_t lun) -> int8_t {
        return USBD_OK;
    },

    //Read
    [](uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len) -> int8_t {
        memcpy(buf, ramdisk + blk_addr * blockSize, blk_len * blockSize);
        return USBD_OK;
    },

    //Write
    [](uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len) -> int8_t {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        memcpy(ramdisk + blk_addr * blockSize, buf, blk_len * blockSize);
        return USBD_OK;
    },

    //GetMaxLun
    []() -> int8_t {return 0;},

    (int8_t *)MSC_Inquirydata

};

}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
 
}
```
