/****************************************************************************
 *
 * USBSerial core library for Arduino STM32 + HAL + CubeMX (HALMX).
 *
 * Copyright (c) 2016 by Vassilis Serasidis <info@serasidis.gr>
 * Home: http://www.serasidis.gr
 * email: avrsite@yahoo.gr
 *
 * Arduino_STM32 forum: http://www.stm32duino.com
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 *
 * Some functions follow the sam and samd arduino core libray files.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
 * BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 *
 ****************************************************************************/

#include "SerialUSB.h"
#include "STM32System.h"
#include "variant.h"

#include "USBDevice.h"

// Constructors ////////////////////////////////////////////////////////////////
SerialUSBClass::SerialUSBClass(){
  // Make sure Rx ring buffer is initialized back to empty.
  rx_buffer.iHead = rx_buffer.iTail = 0;
  //tx_buffer.iHead = tx_buffer.iTail = 0;
}

void SerialUSBClass::init(void) {
    if (stdoutStream == NULL) {
        #if defined(MENU_SERIAL) && MENU_SERIAL == SerialUSB
            stdoutStream = this;
        #elif defined(MENU_SERIAL_AUTO) && MENU_SERIAL_AUTO == SerialUSB
            stdoutStream = this;
        #endif
    }
}


void SerialUSBClass::begin(uint32_t baud_count){
  init();
  // suppress "unused parameter" warning
	(void)baud_count;
}

void SerialUSBClass::begin(uint32_t baud_count, uint8_t config){
  init();
	//suppress "unused parameter" warning
	(void)baud_count;
	(void)config;
}

void SerialUSBClass::end(void){

}


int SerialUSBClass::availableForWrite(void){
  //return (CDC_SERIAL_BUFFER_SIZE - available());
  //return (uint32_t)(CDC_SERIAL_BUFFER_SIZE + tx_buffer.iHead - tx_buffer.iTail) % CDC_SERIAL_BUFFER_SIZE;
  return 0;
}


int SerialUSBClass::available(void){
  return (uint32_t)(CDC_SERIAL_BUFFER_SIZE + rx_buffer.iHead - rx_buffer.iTail) % CDC_SERIAL_BUFFER_SIZE;
}

int SerialUSBClass::peek(void)
{
  if ( rx_buffer.iHead == rx_buffer.iTail )
    return -1;

  return rx_buffer.buffer[rx_buffer.iTail];
}

int SerialUSBClass::read(void)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if ( rx_buffer.iHead == rx_buffer.iTail )
    return -1;

  uint8_t uc = rx_buffer.buffer[rx_buffer.iTail];
  rx_buffer.iTail = (unsigned int)(rx_buffer.iTail + 1) % CDC_SERIAL_BUFFER_SIZE;

  return uc;
}

void SerialUSBClass::flush(void){
  //It's not implemented yet.
}

size_t SerialUSBClass::write(const uint8_t *buffer, size_t size){
   unsigned long timeout=millis()+5;

    if(hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED) {
        return 0;
    }

   for(size_t i=0; i < size; i++) {

       tx_buffer.buffer[tx_buffer.iHead] = *buffer;
       tx_buffer.iHead = (tx_buffer.iHead + 1) % sizeof(tx_buffer.buffer);
       buffer++;

       while(tx_buffer.iHead == tx_buffer.iTail && millis()<timeout);
       if (tx_buffer.iHead == tx_buffer.iTail) break;
   }

   if (transmitting == 0) {
       if (tx_buffer.iHead > tx_buffer.iTail) {
           transmitting = tx_buffer.iHead - tx_buffer.iTail;
       } else {
           transmitting = sizeof(tx_buffer.buffer) - tx_buffer.iTail;
       }
       CDC_Transmit_FS(&tx_buffer.buffer[tx_buffer.iTail], transmitting);

       return size;
   }

  return 0;

}

size_t SerialUSBClass::write(uint8_t c) {
  return write(&c, 1);
}

void SerialUSBClass::CDC_TxHandler() {
  tx_buffer.iTail += transmitting;
  tx_buffer.iTail = tx_buffer.iTail % sizeof(tx_buffer.buffer);

  if (tx_buffer.iHead != tx_buffer.iTail) {
      if (tx_buffer.iHead > tx_buffer.iTail) {
         transmitting = tx_buffer.iHead - tx_buffer.iTail;
     } else {
         transmitting = sizeof(tx_buffer.buffer) - tx_buffer.iTail;
     }
     CDC_Transmit_FS(tx_buffer.buffer + tx_buffer.iTail, transmitting);
  } else {
      transmitting = 0;
  }
}

void SerialUSBClass::CDC_RxHandler (uint8_t* Buf, uint16_t Len){

  for(uint16_t i=0;i<Len;i++){
    if(available() < (CDC_SERIAL_BUFFER_SIZE - 1)){
      rx_buffer.buffer[rx_buffer.iHead] = *Buf++;
      rx_buffer.iHead = (uint16_t)(rx_buffer.iHead + 1) % CDC_SERIAL_BUFFER_SIZE;
    }else
      break;
  }
}


// This operator is a convenient way for a sketch to check whether the
// port has actually been configured and opened by the host (as opposed
// to just being connected to the host).  It can be used, for example, in
// setup() before printing to ensure that an application on the host is
// actually ready to receive and display the data.
// We add a short delay before returning to fix a bug observed by Federico
// where the port is configured (lineState != 0) but not quite opened.
SerialUSBClass::operator bool()
{
	// this is here to avoid spurious opening after upload
	if (millis() < 500)
		return false;

	bool result = false;
	if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED){
      result = true;
	}
/* 	if (_usbLineInfo.lineState > 0)
	{
		result = true;
	}

	delay(10); */
	return result;
}

uint32_t SerialUSBClass::baud() {
	//return _usbLineInfo.dwDTERate;
  return 0;
}

uint8_t SerialUSBClass::stopbits() {
	//return _usbLineInfo.bCharFormat;
  return 0;
}

uint8_t SerialUSBClass::paritytype() {
	//return _usbLineInfo.bParityType;
  return 0;
}

uint8_t SerialUSBClass::numbits() {
	//return _usbLineInfo.bDataBits;
  return 0;
}

bool SerialUSBClass::dtr() {
	//return _usbLineInfo.lineState & 0x1;
  return 0;
}

bool SerialUSBClass::rts() {
	//return _usbLineInfo.lineState & 0x2;
  return 0;
}

extern "C" void USBSerial_Tx_Handler(){
  SerialUSB.CDC_TxHandler();
}

extern "C" void USBSerial_Rx_Handler(uint8_t *data, uint16_t len){
  SerialUSB.CDC_RxHandler(data, len);
}

SerialUSBClass SerialUSB;
