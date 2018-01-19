#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>

extern void (*exec_fun)(void);
extern uint16_t nnn;
extern uint8_t nn, n, *vx, *vy;

void fetch_and_decode(void);

void decode_0nnn(uint16_t opcode);
void decode_0x00(uint16_t opcode);
void decode_0xnn(uint16_t opcode);
void decode_0xy0(uint16_t opcode);
void decode_0xyn(uint16_t opcode);

#endif
