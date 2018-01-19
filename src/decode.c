#include <decode.h>
#include <stddef.h>
#include <execute.h>
#include <machine.h>

struct opcode_enc {
    uint16_t mask;
    uint16_t code;
    void (*decode_fun)(uint16_t opcode);
    void (*exec_fun)(void);
};

const struct opcode_enc opcode_encs[] = {
    {0xFFFF, 0x00E0, NULL,        exec_00e0},
    {0xFFFF, 0x00EE, NULL,        exec_00ee},
    {0xF000, 0x1000, decode_0nnn, exec_1nnn},
    {0xF000, 0x2000, decode_0nnn, exec_2nnn},
    {0xF000, 0x3000, decode_0xnn, exec_3xnn},
    {0xF000, 0x4000, decode_0xnn, exec_4xnn},
    {0xF00F, 0x5000, decode_0xy0, exec_5xy0},
    {0xF000, 0x6000, decode_0xnn, exec_6xnn},
    {0xF000, 0x7000, decode_0xnn, exec_7xnn},
    {0xF00F, 0x8000, decode_0xy0, exec_8xy0},
    {0xF00F, 0x8001, decode_0xy0, exec_8xy1},
    {0xF00F, 0x8002, decode_0xy0, exec_8xy2},
    {0xF00F, 0x8003, decode_0xy0, exec_8xy3},
    {0xF00F, 0x8004, decode_0xy0, exec_8xy4},
    {0xF00F, 0x8005, decode_0xy0, exec_8xy5},
    {0xF00F, 0x8006, decode_0xy0, exec_8xy6},
    {0xF00F, 0x8007, decode_0xy0, exec_8xy7},
    {0xF00F, 0x800E, decode_0xy0, exec_8xye},
    {0xF00F, 0x9000, decode_0xy0, exec_9xy0},
    {0xF000, 0xA000, decode_0nnn, exec_annn},
    {0xF000, 0xB000, decode_0nnn, exec_bnnn},
    {0xF000, 0xC000, decode_0xnn, exec_cxnn},
    {0xF000, 0xD000, decode_0xyn, exec_dxyn},
    {0xF0FF, 0xE09E, decode_0x00, exec_ex9e},
    {0xF0FF, 0xE0A1, decode_0x00, exec_exa1},
    {0xF0FF, 0xF007, decode_0x00, exec_fx07},
    {0xF0FF, 0xF00A, decode_0x00, exec_fx0a},
    {0xF0FF, 0xF015, decode_0x00, exec_fx15},
    {0xF0FF, 0xF018, decode_0x00, exec_fx18},
    {0xF0FF, 0xF01E, decode_0x00, exec_fx1e},
    {0xF0FF, 0xF029, decode_0x00, exec_fx29},
    {0xF0FF, 0xF033, decode_0x00, exec_fx33},
    {0xF0FF, 0xF055, decode_0x00, exec_fx55},
    {0xF0FF, 0xF065, decode_0x00, exec_fx65}
};

const size_t n_opcode_encs = sizeof(opcode_encs)/sizeof(struct opcode_enc);

void (*exec_fun)(void);
uint16_t nnn;
uint8_t nn, n, *vx, *vy;

void fetch_and_decode(void)
{
    uint16_t opcode = (uint16_t)mem[pc++] << 8;
    opcode |= (uint16_t)mem[pc++];

    for(size_t i = 0; i < n_opcode_encs; i++){
        const struct opcode_enc *cand_enc = &opcode_encs[i];

        if(cand_enc->code == (opcode & cand_enc->mask)){
            exec_fun = cand_enc->exec_fun;

            if(cand_enc->decode_fun != NULL){
                cand_enc->decode_fun(opcode);
            }

            return;
        }
    }

    exec_fun = NULL;
}

void decode_0nnn(uint16_t opcode)
{
    nnn = opcode & 0x0FFF;
}

void decode_0x00(uint16_t opcode)
{
    uint8_t x = (opcode >> 8) & 0xF;
    vx = &regs[x];
}

void decode_0xnn(uint16_t opcode)
{
    decode_0x00(opcode);

    nn = opcode & 0xFF;
}

void decode_0xy0(uint16_t opcode)
{
    uint8_t x = (opcode >> 8) & 0xF;
    uint8_t y = (opcode >> 4) & 0xF;
    vx = &regs[x];
    vy = &regs[y];
}

void decode_0xyn(uint16_t opcode)
{
    decode_0xy0(opcode);

    n = opcode & 0xF;
}
