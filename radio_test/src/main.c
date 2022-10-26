#include <stdio.h>
#include "STM32F401RE_GPIO.h"
#include "STM32F401RE_SPI.h"
#include "STM32F401RE_RCC.h"

/////////////////////////////////////////////////////////////////////
// SPI Functions
/////////////////////////////////////////////////////////////////////

void spiWrite(uint8_t address, uint8_t value) {
  uint8_t hi, lo;
  digitalWrite(GPIOB, 5, 0); // pulse chip select
  hi = spiSendReceive(address);
  lo = spiSendReceive(value); 
  digitalWrite(GPIOB, 5, 1); // release chip select
	// discard returned values on a write transaction
}

uint8_t spiRead(uint8_t address) {
  uint8_t hi, lo;
  digitalWrite(GPIOB, 5, 0); // pulse chip select
  hi = spiSendReceive(address | 1 << 7); // set msb for reads
  lo = spiSendReceive(0x00);             // send dummy payload 
  digitalWrite(GPIOB, 5, 1); // release chip select
  return lo;
}

void ms_delay(int ms) {
   while (ms-- > 0) {
      volatile int x=1000;
      while (x-- > 0)
         __asm("nop");
   }
}

/////////////////////////////////////////////////////////////////////
// Main Loop
/////////////////////////////////////////////////////////////////////

int main(void) {
  uint8_t debug;
	int16_t x,y;

	//setup clocks and hardware
	spiInit(15, 0, 0); // Initialize SPI pins and clocks
  digitalWrite(GPIOB, 3, 1); // set CSN high
	digitalWrite(GPIOB, 5, 0); // Manually control CE

  spiWrite(0x00, 0); // CONFIG
  spiWrite(0x01, 0); // EN_AA, no Auto ACK
  spiWrite(0x02, 0); // not enabling any data pipe
  spiWrite(0x03, 0x03); // 5 bytes for RX/TX address
  spiWrite(0x04, 0); // no retransmission
  spiWrite(0x05, 0); // RF_CH
  spiWrite(0x06, 0x0E); // RF_SETUP, power = 0db, data rate = 2Mbps

  digitalWrite(GPIOB, 5, 1); // Manually control CE


  while(1) {

  }
}
