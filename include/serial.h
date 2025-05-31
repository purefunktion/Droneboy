#ifndef SERIAL_H
#define SERIAL_H

extern uint8_t serialBuffer[256];
extern uint8_t serialBufferPosition;
extern uint8_t serialBufferReadPosition;

void serial_isr(void) __interrupt;

#endif