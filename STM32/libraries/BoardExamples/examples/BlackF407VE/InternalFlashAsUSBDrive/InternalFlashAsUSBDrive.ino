/* USB drive based on internal flash
 *
 * Select USB: Mass Storage from the menu
 *
 * EXPERIMENTAL, for testing purposes. Sometimes files may be corrupted.
 *
 * Connect to USB. Upload files. Eject drive, Disconnect. Reconnect. Files should be there.
 *
 * Accessing files from sketch coming soon...
 */

#include "Arduino.h"

#include "STM32F4FlashBlock.h"
#include "FlashVariables.h"
#include "FlashBlockDevice.h"
#include "MassStorage.h"

#define FLASHDRIVE_SIZE    64 * 1024 // Half of one flash page is about OK for drive size
#define FLASHDIVE_BLOCKS   FLASHDRIVE_SIZE / 512

//2 x 128KB flash pages (see reference manual / flash module organization)
STM32F4EmbeddedFlashBlock block0(FLASH_BASE + 2 * 128 * 1024, 128 * 1024);
STM32F4EmbeddedFlashBlock block1(FLASH_BASE + 3 * 128 * 1024, 128 * 1024);

// combine them into one virtual storage of 512 bytes
FlashVariables<512> storageBackend(&block0, &block1);

// Use virtual storage as USB drive
FlashBlockDevice flashBlockDevice(FLASHDIVE_BLOCKS, &storageBackend);

// When we open the first time, format it
void formatListener() {
    formatFat12(&flashBlockDevice, FLASHDIVE_BLOCKS);
}

//Called by USB subsystem
BlockDevice *getMassStorage() {
    storageBackend.setFormatListener(formatListener);
    return &flashBlockDevice;
}


void setup() {
    //If the drive appears unformatted, try this (should not be needed):
    //formatFat12(&flashBlockDevice, FLASHDIVE_BLOCKS);

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(1000);
}
