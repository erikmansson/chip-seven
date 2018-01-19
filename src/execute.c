#include <execute.h>
#include <stdlib.h>
#include <decode.h>
#include <machine.h>

void exec_00e0(void)
{
    for(size_t i = 0; i < DISP_SIZE; i++){
        disp[i] = 0;
    }
}

void exec_00ee(void)
{
    pc = stack[sp--];
}

void exec_1nnn(void)
{
    pc = nnn;
}

void exec_2nnn(void)
{
    stack[++sp] = pc;
    pc = nnn;
}

void exec_3xnn(void)
{
    if(*vx == nn) pc += 2;
}

void exec_4xnn(void)
{
    if(*vx != nn) pc += 2;
}

void exec_5xy0(void)
{
    if(*vx == *vy) pc += 2;
}

void exec_6xnn(void)
{
    *vx = nn;
}

void exec_7xnn(void)
{
    *vx += nn;
}

void exec_8xy0(void)
{
    *vx = *vy;
}

void exec_8xy1(void)
{
    *vx |= *vy;
}

void exec_8xy2(void)
{
    *vx &= *vy;
}

void exec_8xy3(void)
{
    *vx ^= *vy;
}

void exec_8xy4(void)
{
    uint16_t r = (uint16_t)(*vx) + (uint16_t)(*vy);
    *vf = r >> 8;
    *vx = (uint8_t)r;
}

void exec_8xy5(void)
{
    *vf = *vx > *vy ? 1 : 0;
    *vx = *vx - *vy;
}

void exec_8xy6(void)
{
    *vf = *vx & 1;
    *vx >>= 1;
}

void exec_8xy7(void)
{
    *vf = *vy > *vx ? 1 : 0;
    *vx = *vy - *vx;
}

void exec_8xye(void)
{
    *vf = *vx >> 7;
    *vx <<= 1;
}

void exec_9xy0(void)
{
    if(*vx != *vy) pc += 2;
}

void exec_annn(void)
{
    addr_reg = nnn;
}

void exec_bnnn(void)
{
    pc = regs[0] + nnn;
}

void exec_cxnn(void)
{
    *vx = nn & (uint8_t)rand();
}

void exec_dxyn(void)
{
    *vf = 0;

    uint16_t sprite_addr = addr_reg;
    size_t bit_offset = *vx % 8;
    size_t x_idx_l = *vx / 8;
    size_t y_idx_r = (x_idx_l + 1) % DISP_SIZE_X;

    for(size_t i = 0; i < n; i++){
        uint8_t sprite = mem[sprite_addr++];
        size_t y_idx = (*vy + i) * DISP_SIZE_X;

        size_t idx_l = y_idx + x_idx_l;
        size_t idx_r = y_idx + y_idx_r;

        uint8_t sprite_l = sprite >> bit_offset;
        uint8_t sprite_r = sprite << (8 - bit_offset);

        if(disp[idx_l] & sprite_l || disp[idx_r] & sprite_r){
            *vf = 1;
        }

        disp[idx_l] ^= sprite_l;
        disp[idx_r] ^= sprite_r;
    }
}

void exec_ex9e(void)
{
    if(keys & (1 << *vx)) pc += 2;
}

void exec_exa1(void)
{
    if(!(keys & (1 << *vx))) pc += 2;
}

void exec_fx07(void)
{
    *vx = delay_timer;
}

void exec_fx0a(void)
{
    if(keys){
        *vx = 0;
        while(!((keys >> *vx) & 1)){
            (*vx)++;
        }
    }else{
        pc -= 2;
    }
}

void exec_fx15(void)
{
    delay_timer = *vx;
}

void exec_fx18(void)
{
    sound_timer = *vx;
}

void exec_fx1e(void)
{
    uint32_t r = (uint32_t)addr_reg + (uint32_t)(*vx);
    *vf = r >> 16;
    addr_reg = (uint16_t)r;
}

void exec_fx29(void)
{
    addr_reg = SPRITE_START_ADDR + 5 * (*vx);
}

void exec_fx33(void)
{
    mem[addr_reg + 0] = *vx / 100;
    mem[addr_reg + 1] = (*vx / 10) % 10;
    mem[addr_reg + 2] = *vx % 10;
}

void exec_fx55(void)
{
    uint8_t *v = &regs[0];

    while(v <= vx){
        mem[addr_reg++] = *v++;
    }
}

void exec_fx65(void)
{
    uint8_t *v = &regs[0];

    while(v <= vx){
        *v++ = mem[addr_reg++];
    }
}
