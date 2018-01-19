#ifndef MACHINE_H
#define MACHINE_H

#include <stdint.h>
#include <stddef.h>

#define MEM_SIZE 0x1000
#define PROG_START_ADDR 0x200
#define SPRITE_START_ADDR 0x0

#define DISP_WIDTH 64
#define DISP_HEIGHT 32
#define DISP_SIZE_X 8
#define DISP_SIZE 256

extern uint8_t *disp;
extern uint16_t keys;
extern uint8_t mem[];
extern uint8_t regs[];
extern uint16_t addr_reg;
extern uint16_t stack[];
extern uint8_t delay_timer, sound_timer;
extern uint16_t pc;
extern int sp;

extern uint8_t * const vf;

void init_machine(void);
void perform_cycle(void);
void decr_timers(void);

#endif
