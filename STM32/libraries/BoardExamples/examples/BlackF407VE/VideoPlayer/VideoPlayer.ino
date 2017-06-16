/**

  Play raw rgb565 on ILI9341 display, wav on MAX98357A from microSD in the onboard slot
  
  Create video file: ffmpeg -i in.avi -f rawvideo -pix_fmt rgb565 -s 320x240 -vcodec rawvideo video.raw
  Create audio file: ffmpeg -i in.avi -ar 44100 audio.wav

  Example files: http://danieleff.com/stm32/test/
  
  Put the resulting files into sdcard root
  
  Install Adafruit_GFX and Adafruit_ILI9341
  
  Connect ILI9341 and MAX98357A, see below
  
*/

#include "SdFat.h"

#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#include "I2S.h"

#define TFT_DC PA2
#define TFT_CS PA0
#define TFT_RESET PA1

// SPI is on PA5/PA6/PA7
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RESET);

I2SClass I2S(SPI2, PC3 /*DIN*/ , PB12 /*LRC*/, PB10 /*SCLK*/);

SdFatSdio sd;

File videoFile;
File audioFile;

const int VIDEO_FPS = 25;

const int frameBytes = 320 * 240 * 2;

const int videoBufferBytes = 1024 * 32;

uint16_t videoBuffer[2][videoBufferBytes / sizeof(uint16_t)];


const int audioBufferBytes = 1024 * 32;

int16_t audioBuffer[audioBufferBytes / sizeof(uint16_t)];

uint32_t audioSample = 0;
uint32_t audioSampleMillis = 0;

uint32_t lastFpsMillis = 0;
uint32_t fps = 0;

void setup() {
  Serial.begin(115200);

  SPI.stm32SetSCK(PA5);
  SPI.stm32SetMISO(PA6);
  SPI.stm32SetMOSI(PA7);
  tft.begin();

  tft.setRotation(3);

  tft.fillScreen(ILI9341_BLACK);

  I2S.begin(I2S_PHILIPS_MODE, 44100, 16);

  I2S.setBuffer(audioBuffer, audioBufferBytes / sizeof(uint16_t));


  sd.begin();

  videoFile = sd.open("video.raw");
  audioFile = sd.open("audio.wav");

  audioFile.seekSet(44);

  tft.startWrite();

  SPI.beginTransaction(SPISettings(100000000, MSBFIRST, SPI_MODE0)); // Max speeeed
}

void readAudioIntoBuffer() {
    static int16_t buf[1024 * 4];

    uint32_t soundSamplesAvailable = I2S.availableForWrite();

    if (soundSamplesAvailable >= 512) {
        int readBytes = min(soundSamplesAvailable * sizeof(uint16_t), sizeof(buf));

        if (audioFile.read(buf, readBytes) == EOF) {
            while(1); // The end...
        }
        for(int i=0; i<readBytes / 2; i++) {
            buf[i] /= 4; // Volume control
        }

        I2S.write(buf, readBytes / 2);

        audioSample += readBytes / 2;
        audioSampleMillis = millis();
    }
}

void loop() {
  uint32_t audioSampleWithI2SDelay = audioSample + I2S.getDelay();

  uint32_t videoFrame = audioSampleWithI2SDelay * VIDEO_FPS / (44100 * 2);
  videoFrame += (millis() - audioSampleMillis) * VIDEO_FPS / 1000;

  videoFile.seekSet(videoFrame * 320 * 240 * 2);

  int bufferIndex = 0;

  int offset = 0;

  SPI.flush();
  tft.setAddrWindow(0, 0, 320, 240);

  while(offset < frameBytes) {
      readAudioIntoBuffer();

      int readBytes = min(frameBytes - offset, videoBufferBytes);

      videoFile.read(videoBuffer[bufferIndex], readBytes);

      SPI.flush();

      SPI.transfer16(videoBuffer[bufferIndex], NULL, readBytes / sizeof(uint16_t), NULL);

      offset += readBytes;

      bufferIndex++;
      bufferIndex %= 2;
  }

  fps++;
  if (lastFpsMillis + 1000 < millis()) {
      Serial.print("FPS: ");
      Serial.println(fps);

      lastFpsMillis = millis();
      fps = 0;
  }

}
