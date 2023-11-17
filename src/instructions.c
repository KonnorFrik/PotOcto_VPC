#include "instructions.h"

void memory_instructions(PC* vpc, word code, word op1, word op2) {
    word sub_code = code & 0x0f;
    dword addr = 0;

    switch (sub_code) {
        case 0x00:
            addr = ((op1 << 8) | (op2));
            vpc->cpu.MP = addr;

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: MP = %x\n", addr);
            }
            break;
        
        case 0x01:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d) = Mem[%x](%d)\n", op2, vpc->cpu.reg[op2], vpc->cpu.MP, vpc->memory[vpc->cpu.MP]);
            }

            vpc->cpu.reg[op2] = vpc->memory[vpc->cpu.MP];
            break;
        
        case 0x02:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: Mem[%x] = #%x(%d)\n", vpc->cpu.MP, op2, vpc->cpu.reg[op2]);
            }

            vpc->memory[vpc->cpu.MP] = vpc->cpu.reg[op2];
            break;
        
        case 0x03:
            vpc->cpu.MP++;

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: MP++\n");
            }
            break;
        
        case 0x04:
            vpc->cpu.MP--;

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: MP--\n");
            }
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

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d) == #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
            }
            break;

        case 0x01:
            if (vpc->cpu.reg[op1] != vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d) != #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
            }
            break;

        case 0x02:
            if (vpc->cpu.reg[op1] >= vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d) >= #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
            }
            break;

        case 0x03:
            if (vpc->cpu.reg[op1] <= vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d) <= #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
            }
            break;

        case 0x04:
            if (vpc->cpu.reg[op1] > vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d) > #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
            }
            break;

        case 0x05:
            if (vpc->cpu.reg[op1] < vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d) < #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
            }
            break;

        case 0x06:
            if (vpc->cpu.reg[op1] == op2) {
                vpc->cpu.IP += 3;
            }

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d) == (%d)\n", op1, vpc->cpu.reg[op1], op2);
            }
            break;

        case 0x07:
            if (vpc->cpu.reg[op1] != op2) {
                vpc->cpu.IP += 3;
            }

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d) != (%d)\n", op1, vpc->cpu.reg[op1], op2);
            }
            break;

        case 0x08:
            if (vpc->cpu.reg[op1] >= op2) {
                vpc->cpu.IP += 3;
            }

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d) >= (%d)\n", op1, vpc->cpu.reg[op1], op2);
            }
            break;

        case 0x09:
            if (vpc->cpu.reg[op1] <= op2) {
                vpc->cpu.IP += 3;
            }

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d) <= (%d)\n", op1, vpc->cpu.reg[op1], op2);
            }
            break;

        case 0x0a:
            if (vpc->cpu.reg[op1] > op2) {
                vpc->cpu.IP += 3;
            }

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d) > (%d)\n", op1, vpc->cpu.reg[op1], op2);
            }
            break;

        case 0x0b:
            if (vpc->cpu.reg[op1] < op2) {
                vpc->cpu.IP += 3;
            }

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d) < (%d)\n", op1, vpc->cpu.reg[op1], op2);
            }
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

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: IP = %x\n", addr);
            }
            break;

        case 0x01:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d) = #%x(%d)\n", op1, op1, op2, op2);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op2];
            break;

        case 0x02:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d) = (%d)\n", op1, op1, op2);
            }

            vpc->cpu.reg[op1] = op2;
            break;

        case 0x03:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d)++\n", op2, vpc->cpu.reg[op2]);
            }

            (vpc->cpu.reg[op2])++;
            break;

        case 0x04:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x(%d)--\n", op2, vpc->cpu.reg[op2]);
            }

            (vpc->cpu.reg[op2])--;
            break;
    }
}

void math_instructions(PC* vpc, word code, word op1, word op2) {
    word sub_code = code & 0x0f;

    switch (sub_code) {
        case 0x00:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x = #%x(%d) + #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] + vpc->cpu.reg[op2];
            break;

        case 0x01:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x = #%x(%d) - #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] - vpc->cpu.reg[op2];
            break;

        case 0x02:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x = #%x(%d) * #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] * vpc->cpu.reg[op2];
            break;

        case 0x03:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x = #%x(%d) / #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] / vpc->cpu.reg[op2];
            break;

        case 0x04:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x = #%x(%d) %c #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], '%', op2, vpc->cpu.reg[op2]);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] % vpc->cpu.reg[op2];
            break;

        case 0x05:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x = #%x(%d) + (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] + op2;
            break;

        case 0x06:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x = #%x(%d) - (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] - op2;
            break;                                 
                                                   
        case 0x07:                                 
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x = #%x(%d) * (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] * op2;
            break;                                 
                                                   
        case 0x08:                                 
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x = #%x(%d) / (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] / op2;
            break;                                 
                                                   
        case 0x09:                                 
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: #%x = #%x(%d) %c (%d)\n", op1, op1, vpc->cpu.reg[op1], '%', op2);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] % op2;
            break;
    }
}

void bitwise_instructions(PC* vpc, word code, word op1, word op2) {
    word sub_code = code & 0x0f;

    switch (sub_code) {
        case 0x00:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: AND #%x = #%x(%d) & #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] & vpc->cpu.reg[op2];
            break;

        case 0x01:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: OR #%x = #%x(%d) | #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] | vpc->cpu.reg[op2];
            break;

        case 0x02:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: XOR #%x = #%x(%d) ^ #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] ^ vpc->cpu.reg[op2];
            break;

        case 0x03:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: INV #%x = ~#%x(%d)\n", op1, op2, vpc->cpu.reg[op2]);
            }

            vpc->cpu.reg[op1] = ~vpc->cpu.reg[op2];
            break;

        case 0x04:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: LSH #%x = #%x(%d) << #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] << vpc->cpu.reg[op2];
            break;

        case 0x05:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: RSH #%x = #%x(%d) >> #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] >> vpc->cpu.reg[op2];
            break;

        case 0x06:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: AND #%x = #%x(%d) & (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] & op2;
            break;

        case 0x07:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: OR #%x = #%x(%d) | (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] | op2;
            break;

        case 0x08:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: XOR #%x = #%x(%d) ^ (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] ^ op2;
            break;

        case 0x09:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: INV #%x = ~(%d)\n", op1, op2);
            }

            vpc->cpu.reg[op1] = ~op2;
            break;

        case 0x0a:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: LSH #%x = #%x(%d) << (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] << op2;
            break;

        case 0x0b:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Instruction: RSH #%x = #%x(%d) >> (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
            }

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] >> op2;
            break;
    }
}

int do_instruction(PC* vpc, word code, word op1, word op2) {
    word instr_code = code & 0xf0;
    int result = 1;

    if (DEBUG) {
        fprintf(stderr, "\t[DEBUG]: code: %x | op1: %x | op2: %x\n", instr_code, op1, op2);
    }

    switch(instr_code) {
        case MEMORY_SECTION:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Memory section\n");
            }

            memory_instructions(vpc, code, op1, op2);
            break;

        case LOGIC_SECTION:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Logic section\n");
            }

            logic_instructions(vpc, code, op1, op2);
            break;

        case STUFF_SECTION:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Stuff section\n");
            }

            stuff_instructions(vpc, code, op1, op2);
            break;

        case MATH_SECTION:
            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Math section\n");
            }

            math_instructions(vpc, code , op1, op2);
            break;

        case BITWISE_SECTION:
            if (code == 0xff && op1 == 0xff && op2 == 0xff) {
                if (DEBUG) {
                    fprintf(stderr, "\t[DEBUG]: Instruction: HALT\n");
                }

                result = 0;
                break;
            }

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Bitwise section\n");
            }

            bitwise_instructions(vpc, code, op1, op2);
            break;
    }

    if (DEBUG) {
        fprintf(stderr, "\n");
    }

    return result;
}
