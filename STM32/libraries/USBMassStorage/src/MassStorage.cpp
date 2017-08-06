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



const unsigned char fat12BootSector[] = {
    0xEB, 0x3C, 0x90, //jump to boot program
    'S', 'T', 'M', '3', '2', 'F', 'A', 'T', //manufacturer description
    0x00, 0x02, // 512 bytes per block
    0x01, // blocks per allocation unit
    0x01, 0x00, // 1 reserved block for boot block
    0x02, // number of FAT
    0x10, 0x00, // max number of root dir entries
    0x42, 0x00, // number of blocks in the entire disk
    0xF8, // media descriptor

    0x01, 0x00, // blocks for FAT
    0x00, 0x00, // for boot program
    0x00, 0x00, // for boot program
    0x00, 0x00, 0x00, 0x00, // hidden blocks
    0x00, 0x00, 0x00, 0x00, // total number of blocks
    0x00, 0x00, // for boot program
    0x00, //for boot program
    0x38, 0xD3, 0xC7, 0x53, // Disk ID
    'S', 'T', 'M', '3', '2', ' ', 'D', 'r', 'i', 'v', 'e', // partition volume label
    'F', 'A', 'T', '1', '2', ' ', ' ', ' ', // file system type
};

void formatFat12(BlockDevice *blockDevice, uint32_t blockCount) {
    const uint8_t blocksToWrite = 3;
    uint8_t buffer[512 * blocksToWrite] = {0};

    memcpy(buffer, fat12BootSector, sizeof(fat12BootSector));

    buffer[19] = blockCount & 0xFF;
    buffer[20] = blockCount >> 8;

    buffer[510] = 0x55;
    buffer[511] = 0xAA;


    buffer[1024 + 0] = 'S';
    buffer[1024 + 1] = 'T';
    buffer[1024 + 2] = 'M';
    buffer[1024 + 3] = '3';
    buffer[1024 + 4] = '2';
    for(int i = 5; i < 11; i++) {
        buffer[1024 + i] = ' ';
    }
    buffer[1024 + 11] = 0x8; // Volume label dir entry

    blockDevice->writeBlocks(0, buffer, blocksToWrite);
}
