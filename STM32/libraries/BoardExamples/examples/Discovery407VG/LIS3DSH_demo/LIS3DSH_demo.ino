/*
 * Simple demo to test LIS3DSH found in STM32F4 Discovery board
 * CS pin = PE3
 * Use Arduino IDE serial plotter to visualize
 * 
 * leds are used to visualize sensor position
 */

#include "Arduino.h"
#include <SPI.h>

#define RD 0x80
#define WR 0x00

#define CTRL_REG4 0x20
#define CTRL_REG5 0x24
#define CTRL_REG3 0x23

#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

#define WHO_AM_I 0x0F

void readReg(uint8_t addr, uint8_t* data) {
  digitalWrite(PE3, LOW);
  uint8_t ret =SPI.transfer(addr|RD);   
  delayMicroseconds(50);
  ret =SPI.transfer(0x00);   
  delayMicroseconds(50);
  //Serial.print("R:");
  //Serial.println(ret,HEX); 
  digitalWrite(PE3, HIGH); 
  *data=ret;
}

void printXYZ() {
 
  uint8_t xl;
  readReg(OUT_X_L, &xl); 
 
  uint8_t xh;
  readReg(OUT_X_H, &xh); 
 
  int16_t X = (xh << 8) | (xl); 
  
    if (X<0) {
    analogWrite(PD12,X*-1/64);
    analogWrite(PD14,0);
  } else {    
    analogWrite(PD14,X/64);
    analogWrite(PD12,0);
  }

  uint8_t yl;
  readReg(OUT_Y_L, &yl); 
 
  uint8_t yh;
  readReg(OUT_Y_H, &yh); 
 
  int16_t Y = (yh << 8) | (yl); 

  if (Y<0) {
    analogWrite(PD15,Y*-1/64);
    analogWrite(PD13,0);
  } else {    
    analogWrite(PD13,Y/64);
    analogWrite(PD15,0);
  }

  uint8_t zl;
  readReg(OUT_Z_L, &zl); 
 
  uint8_t zh;
  readReg(OUT_Z_H, &zh); 
 
  int16_t Z = (zh << 8) | (zl); 
  
  
  Serial.print(X);
  Serial.print(",");
  Serial.print(Y);
  Serial.print(",");
  Serial.println(Z);
 
}

void initSensor() {

 /* 
 
 taken from datasheet, init with two commands:
  
  1. Write CTRL_REG4 = 67h
  X, Y, Z enabled, ODR = 100 Hz
  
  2. Write CTRL_REG3 = C8h
  DRY active high on INT1 pin

 */

  Serial.println("init LIS3DSH Sensor");
  SPI.begin(); 
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  //SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
 
  digitalWrite(PE3, LOW);
  uint8_t ret =SPI.transfer(CTRL_REG4); 
  delay(1);
  ret =SPI.transfer(0x77);  

  //ret =SPI.transfer(CTRL_REG5); 
  //delay(10);
  //ret =SPI.transfer(0x0);  
 
  digitalWrite(PE5, HIGH);
  
  /* read ID, expect 0x3F for LIS3DSH */
  uint8_t id;
  readReg(WHO_AM_I, &id); 
  
  Serial.print("ID=");
  Serial.println(id, HEX);
 
  Serial.println("...done");
}

void setup() {
  
  Serial.begin(230400);
  pinMode(PE3, OUTPUT);
  pinMode(PD15, OUTPUT);
  pinMode(PD14, OUTPUT);
  pinMode(PD13, OUTPUT);
  
  initSensor(); 
}


void loop() {
  printXYZ();
  delay(20);
}

