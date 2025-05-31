#include <gb/gb.h>
#include <gb/isr.h>
#include "serial.h"
#include "globals.h"

uint8_t serialBuffer[256];
uint8_t serialBufferPosition;
uint8_t serialBufferReadPosition;

// this whole thing is a copy from
// https://github.com/tstirrat/mGB/blob/master/Source/io/serial.c
void serial_isr(void) __critical __interrupt {
  serialBufferPosition++; // unsigned overflow from 255 -> 0 is automatic.
  serialBuffer[serialBufferPosition] = rSB;
  // queue another read
  rSC = SIOF_XFER_START | SIOF_CLOCK_EXT;

  // the interrupt can happen over a vblank, and if so, no rendering should
  // happen otherwise we will get unsafe VRAM access.
  // also this prioritises updateMidiBuffer()
  system_idle = 0;
  IF_REG &= ~SIO_IFLAG; // Clear interrupt flag
}

ISR_VECTOR(VECTOR_SERIAL, serial_isr)