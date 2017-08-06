/* USB drive based on internal flash
 *
 * Select USB: Mass Storage from the menu
 *
 * EXPERIMENTAL, for testing purposes.
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * When writing, use eject drive, or the filesystem may get corrupted. In that case, use:
 *   chkdsk <drive> /f
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 * Connect to USB. Upload files. Eject drive, Disconnect. Reconnect. Files should be there.
 *
 * Create a config.txt in, and write 1/2/3 into it. The leds will light up accordingly
 *
 */

#include "STM32F4FlashBlock.h"
#include "FlashVariables.h"
#include "FlashBlockDevice.h"
#include "MassStorage.h"
#include "FlashBlockFat.h"

#define FLASHDRIVE_SIZE    64 * 1024 // Half of one flash page is about OK for drive size
#define FLASHDIVE_BLOCKS   FLASHDRIVE_SIZE / 512

//2 x 128KB flash pages (see reference manual / flash module organization)
STM32F4EmbeddedFlashBlock block0(FLASH_BASE + 2 * 128 * 1024, 128 * 1024);
STM32F4EmbeddedFlashBlock block1(FLASH_BASE + 3 * 128 * 1024, 128 * 1024);

// combine them into one virtual storage of 512 bytes
FlashVariables<512> storageBackend(&block0, &block1);


// SdFat interface, so we can access it from the microcontroller
FlashBlockFat fat(&storageBackend);


// USB interface so we can access it through USB
FlashBlockDevice flashBlockDevice(FLASHDIVE_BLOCKS, &storageBackend);

// When we open the first time, and is not formatted, format it
void formatListener() {
    formatFat12(&flashBlockDevice, FLASHDIVE_BLOCKS);
}

//Called by USB subsystem
BlockDevice *getMassStorage() {
    storageBackend.setFormatListener(formatListener);

    //If the drive appears unformatted, try this (should not be needed):
    //storageBackend.format();

    return &flashBlockDevice;
}

void setup() {
    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED1_BUILTIN, OUTPUT);

    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(LED1_BUILTIN, HIGH);

    delay(1000);

    fat.begin();
}

void loop() {
    delay(10);

    // Needed, or SdFat will not know about changes from USB drive
    fat.cacheClear();


    File f = fat.open("config.txt", FILE_READ);
    if (!f) {
        return;
    }

    char c = f.read();
    Serial.println(c);
    switch(c) {
        case '1':
            digitalWrite(LED_BUILTIN, LOW);
            digitalWrite(LED1_BUILTIN, HIGH);
            break;
        case '2':
            digitalWrite(LED_BUILTIN, HIGH);
            digitalWrite(LED1_BUILTIN, LOW);
            break;
        case '3':
            digitalWrite(LED_BUILTIN, LOW);
            digitalWrite(LED1_BUILTIN, LOW);
            break;
        default:
            digitalWrite(LED_BUILTIN, HIGH);
            digitalWrite(LED1_BUILTIN, HIGH);
            break;
    }
    f.close();

    //
    //delay(1000);
}
