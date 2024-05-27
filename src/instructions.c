#include <stdio.h>

#include "instructions.h"

#define INST_DEBUG 1

void memory_instructions(PC* vpc, word code, word op1, word op2) {
    word sub_code = code & 0x0f;
    dword addr = 0;

    switch (sub_code) {
        case 0x00:
            addr = ((op1 << 8) | (op2));
            vpc->cpu.MP = addr;

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: MP = %x\n", addr);
#endif
            break;
        
        case 0x01:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d) = Mem[%x](%d)\n", op2, vpc->cpu.reg[op2], vpc->cpu.MP, vpc->memory[vpc->cpu.MP]);
#endif

            vpc->cpu.reg[op2] = vpc->memory[vpc->cpu.MP];
            break;
        
        case 0x02:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: Mem[%x] = #%x(%d)\n", vpc->cpu.MP, op2, vpc->cpu.reg[op2]);
#endif

            vpc->memory[vpc->cpu.MP] = vpc->cpu.reg[op2];
            break;
        
        case 0x03:
            vpc->cpu.MP++;

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: MP++\n");
#endif
            break;
        
        case 0x04:
            vpc->cpu.MP--;

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: MP--\n");
#endif
            break;
    }
}

void logic_instructions(PC* vpc, word code, word op1, word op2) {
    word sub_code = code & 0x0f;

    switch (sub_code) {
        case 0x00:
            if (vpc->cpu.reg[op1] == vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d) == #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case 0x01:
            if (vpc->cpu.reg[op1] != vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d) != #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case 0x02:
            if (vpc->cpu.reg[op1] >= vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d) >= #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case 0x03:
            if (vpc->cpu.reg[op1] <= vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d) <= #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case 0x04:
            if (vpc->cpu.reg[op1] > vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d) > #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case 0x05:
            if (vpc->cpu.reg[op1] < vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d) < #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case 0x06:
            if (vpc->cpu.reg[op1] == op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d) == (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;

        case 0x07:
            if (vpc->cpu.reg[op1] != op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d) != (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;

        case 0x08:
            if (vpc->cpu.reg[op1] >= op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d) >= (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;

        case 0x09:
            if (vpc->cpu.reg[op1] <= op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d) <= (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;

        case 0x0a:
            if (vpc->cpu.reg[op1] > op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d) > (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;

        case 0x0b:
            if (vpc->cpu.reg[op1] < op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d) < (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;
    }
}

void stuff_instructions(PC* vpc, word code, word op1, word op2) {
    word sub_code = code & 0x0f;
    dword addr;

    switch (sub_code) {
        case 0x00:
            addr = ((op1 << 16) | (op2));
            vpc->cpu.IP = addr;

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: IP = %x\n", addr);
#endif
            break;

        case 0x01:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d) = #%x(%d)\n", op1, op1, op2, op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op2];
            break;

        case 0x02:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d) = (%d)\n", op1, op1, op2);
#endif

            vpc->cpu.reg[op1] = op2;
            break;

        case 0x03:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d)++\n", op2, vpc->cpu.reg[op2]);
#endif

            (vpc->cpu.reg[op2])++;
            break;

        case 0x04:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x(%d)--\n", op2, vpc->cpu.reg[op2]);
#endif

            (vpc->cpu.reg[op2])--;
            break;
    }
}

void math_instructions(PC* vpc, word code, word op1, word op2) {
    word sub_code = code & 0x0f;

    switch (sub_code) {
        case 0x00:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x = #%x(%d) + #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] + vpc->cpu.reg[op2];
            break;

        case 0x01:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x = #%x(%d) - #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] - vpc->cpu.reg[op2];
            break;

        case 0x02:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x = #%x(%d) * #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] * vpc->cpu.reg[op2];
            break;

        case 0x03:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x = #%x(%d) / #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] / vpc->cpu.reg[op2];
            break;

        case 0x04:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x = #%x(%d) %c #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], '%', op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] % vpc->cpu.reg[op2];
            break;

        case 0x05:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x = #%x(%d) + (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] + op2;
            break;

        case 0x06:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x = #%x(%d) - (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] - op2;
            break;                                 
                                                   
        case 0x07:                                 
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x = #%x(%d) * (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] * op2;
            break;                                 
                                                   
        case 0x08:                                 
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x = #%x(%d) / (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] / op2;
            break;                                 
                                                   
        case 0x09:                                 
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: #%x = #%x(%d) %c (%d)\n", op1, op1, vpc->cpu.reg[op1], '%', op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] % op2;
            break;
    }
}

void bitwise_instructions(PC* vpc, word code, word op1, word op2) {
    word sub_code = code & 0x0f;

    switch (sub_code) {
        case 0x00:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: AND #%x = #%x(%d) & #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] & vpc->cpu.reg[op2];
            break;

        case 0x01:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: OR #%x = #%x(%d) | #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] | vpc->cpu.reg[op2];
            break;

        case 0x02:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: XOR #%x = #%x(%d) ^ #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] ^ vpc->cpu.reg[op2];
            break;

        case 0x03:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: INV #%x = ~#%x(%d)\n", op1, op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = ~vpc->cpu.reg[op2];
            break;

        case 0x04:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: LSH #%x = #%x(%d) << #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] << vpc->cpu.reg[op2];
            break;

        case 0x05:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: RSH #%x = #%x(%d) >> #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] >> vpc->cpu.reg[op2];
            break;

        case 0x06:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: AND #%x = #%x(%d) & (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] & op2;
            break;

        case 0x07:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: OR #%x = #%x(%d) | (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] | op2;
            break;

        case 0x08:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: XOR #%x = #%x(%d) ^ (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] ^ op2;
            break;

        case 0x09:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: INV #%x = ~(%d)\n", op1, op2);
#endif

            vpc->cpu.reg[op1] = ~op2;
            break;

        case 0x0a:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: LSH #%x = #%x(%d) << (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] << op2;
            break;

        case 0x0b:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction: RSH #%x = #%x(%d) >> (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] >> op2;
            break;
    }
}

int do_instruction(PC* vpc, word code, word op1, word op2) {
    word instr_code = code & 0xf0;
    int result = 1;

#if INST_DEBUG == 1
        fprintf(stderr, "\t[INST_DEBUG]: code: %x | op1: %x | op2: %x\n", instr_code, op1, op2);
#endif

    switch(instr_code) {
        case MEMORY_SECTION:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Memory section\n");
#endif

            memory_instructions(vpc, code, op1, op2);
            break;

        case LOGIC_SECTION:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Logic section\n");
#endif

            logic_instructions(vpc, code, op1, op2);
            break;

        case STUFF_SECTION:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Stuff section\n");
#endif

            stuff_instructions(vpc, code, op1, op2);
            break;

        case MATH_SECTION:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Math section\n");
#endif

            math_instructions(vpc, code , op1, op2);
            break;

        case BITWISE_SECTION:
            if (code == 0xff && op1 == 0xff && op2 == 0xff) {
#if INST_DEBUG == 1
                    fprintf(stderr, "\t[INST_DEBUG]: Instruction: HALT\n");
#endif

                result = 0;
                break;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Bitwise section\n");
#endif

            bitwise_instructions(vpc, code, op1, op2);
            break;
    }

#if INST_DEBUG == 1
        fprintf(stderr, "\n");
#endif

    return result;
}
