/*
 SDcard SDIO example - Copy Test - copy binaries
 Buffer of BUFSIZE bytes is used for writing.
 You have to place a binary called "test_in.bin" on the SDcard and run the sketch.
 You will get the "test_out.bin" after the copy finishes.
 Pito 5/2016, no warranties of any kind
 Pito 5/2017, no warranties of any kind
 */
#include "SdFat.h"
#include "CRC32.h"

SdFatSdio sd;

SdFile myFileIn;
SdFile myFileOut;
CRC32 crc;

#define BUFSIZE 32*1024

// Serial output stream
ArduinoOutStream cout(Serial);

void setup() {
    Serial.begin(115200);
    delay(1000);
    cout << F("\nUse a freshly formatted SD for best performance.\n");
    // Discard any input.
    do {
        delay(10);
    } while (Serial.available() && Serial.read() >= 0);

    Serial.println("Type any character to start\n");
    while (!Serial.available()) {
        SysCall::yield();
    }

    if (!sd.begin()) {
        sd.initErrorHalt();
    }
}

void loop() {
    Serial.println("*************************************************");
    Serial.println("Opening the read file..");
    // open the file for Read
    if (!myFileIn.open("test_in.bin", O_RDONLY)) {
        sd.errorHalt("opening test_in.bin for Read failed");
    }

    Serial.println("Opening the write file..");
    // open the file for Write
    if (!myFileOut.open("test_out.bin", O_WRITE | O_CREAT)) {
        sd.errorHalt("opening test_out.bin for Write failed");
    }

    // Read from myFileIn and write to myFileOut via a Buffer
    Serial.println("Reading and Writing..");
    uint8_t buf[BUFSIZE];
    uint32_t nr;
    uint32_t t = millis();
    while ((nr = myFileIn.read(buf, BUFSIZE)) > 0) {
        if (nr < BUFSIZE){
            myFileOut.write(buf, nr);  }
        else {
            myFileOut.write(buf, BUFSIZE);  }
    }
    t = millis() - t;
    // close the files
    myFileIn.close();
    myFileOut.close();
    Serial.println("*************************************************");
    Serial.print("Done in ");
    Serial.print(t);
    Serial.println(" msecs");

    /*
     * CRC Calculations
     */
    Serial.println("*************************************************");
    Serial.println("Running CRC calculations...");
    t = millis();

    if (!myFileIn.open("test_in.bin", O_RDONLY)) {
        sd.errorHalt("opening test_in.bin for CRC calculation failed");
    }
    crc.reset();
    while ((nr = myFileIn.read(buf, BUFSIZE)) > 0) {
            crc.update(buf,nr);
    }
    uint32_t checksumIn = crc.finalize();
    myFileIn.close();

    if (!myFileIn.open("test_out.bin", O_RDONLY)) {
        sd.errorHalt("opening test_out.bin for CRC calculation failed");
    }
    crc.reset();
    while ((nr = myFileIn.read(buf, BUFSIZE)) > 0) {
            crc.update(buf,nr);
    }
    uint32_t checksumOut = crc.finalize();
    t = millis() - t;

    Serial.print("File in CRC:  ");
    Serial.println(checksumIn, HEX);
    Serial.print("File out CRC: ");
    Serial.println(checksumOut, HEX);
    if (checksumIn != checksumOut) {
        Serial.println("**CHECKSUM DID NOT MATCH!");
    }
    Serial.println("*************************************************");
    Serial.print("Done in ");
    Serial.print(t);
    Serial.print(" msecs");

    myFileIn.close();



    Serial.println("Type any character to start\n");
    while (!Serial.available()) {
        SysCall::yield();
    }
    if (!myFileOut.open("test_out.bin", O_RDWR)) {
        sd.errorHalt("opening test_out.bin for delete failed");
    }
    if (!myFileOut.remove()) {
        sd.errorHalt("removing test_out.bin failed");
    }
}
