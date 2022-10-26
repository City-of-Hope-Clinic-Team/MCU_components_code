#include <stdio.h>
#include "STM32F401RE_GPIO.h"
#include "STM32F401RE_SPI.h"
#include "STM32F401RE_RCC.h"

/////////////////////////////////////////////////////////////////////
// SPI Functions
/////////////////////////////////////////////////////////////////////

void spiWrite(uint8_t address, uint8_t value) {
  uint8_t hi, lo;
  digitalWrite(GPIOB, 6, 0); // pulse chip select
  hi = spiSendReceive(address);
  lo = spiSendReceive(value); 
  digitalWrite(GPIOB, 6, 1); // release chip select
	// discard returned values on a write transaction
}

uint8_t spiRead(uint8_t address) {
  uint8_t hi, lo;
  digitalWrite(GPIOB, 6, 0); // pulse chip select
  hi = spiSendReceive(address | 1 << 7); // set msb for reads
  lo = spiSendReceive(0x00);             // send dummy payload 
  digitalWrite(GPIOB, 6, 1); // release chip select
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
  pinMode(GPIOB, 6, GPIO_OUTPUT); 
	digitalWrite(GPIOB, 6, 1); // Manually control LIS3DH Chip Select

  // Check WHO_AM_I register. should return 0x33 = 51 decimal
	// Then do something with debug value to prevent compiler from
	// optimizing it away.
	debug = spiRead(0x0F);
	if (debug == 51) digitalWrite(GPIOA, 2, 1); 

  // Setup the LIS3DH for use
	spiWrite(0x20, 0x77); // highest conversion rate, all axis on
	spiWrite(0x23, 0x88); // block update, and high resolution

  pinMode(GPIOA, 0, GPIO_OUTPUT);
  pinMode(GPIOA, 1, GPIO_OUTPUT);

  while(1) {
    // Collect the X and Y values from the LIS3DH
    x = spiRead(0x28) | (spiRead(0x29) << 8);
    y = spiRead(0x2A) | (spiRead(0x2B) << 8);

    if (y > 0) {
      digitalWrite(GPIOA, 0, 1);
      ms_delay(100);
    }
    if (y < 0) {
      digitalWrite(GPIOA, 1, 1);
      ms_delay(100);
    }
    digitalWrite(GPIOA, 0, 0);
    digitalWrite(GPIOA, 1, 0);
    ms_delay(100);
  }
}
