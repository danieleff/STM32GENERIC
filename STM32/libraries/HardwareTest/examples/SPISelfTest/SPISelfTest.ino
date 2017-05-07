// CONNECT SPI1 SCK -> SPI2 SCK
// CONNECT SPI1 MOSI -> SPI2 MOSI
// CONNECT SPI1 MISO -> SPI2 MISO
// CONNECT master_cs -> SPI2 NSS
uint8_t master_cs = 7;

#include "SPI.h"

const uint32_t buffer_size = 1024;

extern volatile uint32_t slave_index;
extern uint8_t slave_out[buffer_size];
extern uint8_t slave_in[buffer_size];

void slave();
void slave_reset();


uint32_t master_index;
uint8_t master_out[buffer_size];
uint8_t master_in[buffer_size];

void setup() {
  Serial.begin(1);
  
  SPI.begin();
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));

  for(int i=0; i<buffer_size; i++) {
    master_out[i] = 5 + i * 2;
    slave_out[i] = 12 + i * 3;
  }
  slave();
  
  pinMode(master_cs, OUTPUT);
  digitalWrite(master_cs, HIGH);

  delay(3000);
}

void loop() {
  for(int i=0; i<buffer_size; i++) {
    master_in[i] = 200;
    slave_in[i] = 200;
  }
  
  slave_reset();
  
  master_index = 0;
  digitalWrite(master_cs, LOW);
  
  for(int j = 0; j < 2; j++) {
    for(int i=0; i<4; i++) {
      master_in[master_index] = SPI.transfer(master_out[master_index]);
      master_index++;
    }
  
    uint8_t dma_transfer_length = 4;
    SPI.dmaTransfer(master_out + master_index, master_in + master_index, dma_transfer_length);
    master_index += dma_transfer_length;

  
    dma_transfer_length = 4;
    SPI.dmaTransfer(NULL, master_in + master_index, dma_transfer_length);
    master_index += dma_transfer_length;
  
  
    uint8_t dma_send_length = 4;
    SPI.dmaSend(master_out + master_index, dma_send_length);
    master_index += dma_send_length;
  
    for(int i=0; i<4; i++) {
      uint8_t b = master_out[master_index];
      SPI.transfer(&b, 1);
      master_in[master_index] = b;
      master_index++;
    }
  }
  
  Serial.print("Master sent: ");
  Serial.println(master_index);
  
  Serial.print("Slave received: ");
  Serial.println(slave_index);
  
  Serial.println("M out\t|S in\t|\t|S out\t|M in:");
  
  for(int i=0; i<master_index; i++) {
    Serial.print(master_out[i]);
    Serial.print("\t");
    Serial.print(slave_in[i]);
    
    if (master_out[i] != slave_in[i]) Serial.print("!!!");
    
    Serial.print("\t|\t|");
    
    Serial.print(slave_out[i]);
    Serial.print("\t|");
    Serial.print(master_in[i]);
    
    if (slave_out[i] != master_in[i]) Serial.print("!!!");
    
    Serial.println("\t");
  }
  Serial.println("");
  Serial.println("----");
  
  digitalWrite(master_cs, HIGH);

  Serial.println("Press a key to retry");
  
  while(Serial.read() == -1);
  while(Serial.read() != -1);
  
}