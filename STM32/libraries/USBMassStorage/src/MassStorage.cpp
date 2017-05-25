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

#include "USBDevice.h"

#include "BlockDevice.h"

#include "msc/usbd_msc.h"

#include "MassStorage.h"

BlockDevice *usbBlockDevice;

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

USBD_StorageTypeDef USBD_DISK_fops = {
    //Init
    [](uint8_t lun) -> int8_t {
        usbBlockDevice = getMassStorage();

        return usbBlockDevice->begin() ? USBD_OK : USBD_FAIL;
    },

    //GetCapacity
    [](uint8_t lun, uint32_t *block_num, uint16_t *block_size) -> int8_t {
        *block_num  = usbBlockDevice->getBlockCount();
        *block_size = 512;
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
        return usbBlockDevice->readBlocks(blk_addr, buf, blk_len) ? USBD_OK : USBD_FAIL;
    },

    //Write
    [](uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len) -> int8_t {
        return usbBlockDevice->writeBlocks(blk_addr, buf, blk_len) ? USBD_OK : USBD_FAIL;
    },

    //GetMaxLun
    []() -> int8_t {
        return 0;
    },

    (int8_t *)MSC_Inquirydata

};
