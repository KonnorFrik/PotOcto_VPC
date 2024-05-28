#include <stdio.h>

#include "instructions_executors.h"
#include "instructions_map.h"

#define INST_DEBUG 1

void memory_instructions(PC* vpc, word code, word op1, word op2) {
    word sub_code = code & 0x0f;
    dword addr = 0;

    switch (sub_code) {
        case MEM_SET:
            addr = ((op1 << 8) | (op2));
            vpc->cpu.MP = addr;

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction set: MP = %x\n", addr);
#endif
            break;
        
        case MEM_READ:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction read: #%x(%d) = Mem[%x](%d)\n", op2, vpc->cpu.reg[op2], vpc->cpu.MP, vpc->memory[vpc->cpu.MP]);
#endif

            vpc->cpu.reg[op2] = vpc->memory[vpc->cpu.MP];
            break;
        
        case MEM_WRITE:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction write: Mem[%x] = #%x(%d)\n", vpc->cpu.MP, op2, vpc->cpu.reg[op2]);
#endif

            vpc->memory[vpc->cpu.MP] = vpc->cpu.reg[op2];
            break;
        
        case MEM_INC:
            vpc->cpu.MP++;

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction inc: MP++\n");
#endif
            break;
        
        case MEM_DEC:
            vpc->cpu.MP--;

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction dec: MP--\n");
#endif
            break;
    }
}

void logic_instructions(PC* vpc, word code, word op1, word op2) {
    word sub_code = code & 0x0f;

    switch (sub_code) {
        case LOGIC_REG_EQ:
            if (vpc->cpu.reg[op1] == vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg eq: #%x(%d) == #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case LOGIC_REG_NEQ:
            if (vpc->cpu.reg[op1] != vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg neq: #%x(%d) != #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case LOGIC_REG_GE:
            if (vpc->cpu.reg[op1] >= vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg ge: #%x(%d) >= #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case LOGIC_REG_LE:
            if (vpc->cpu.reg[op1] <= vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg le: #%x(%d) <= #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case LOGIC_REG_GT:
            if (vpc->cpu.reg[op1] > vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg gt: #%x(%d) > #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case LOGIC_REG_LT:
            if (vpc->cpu.reg[op1] < vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg lt: #%x(%d) < #%x(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case LOGIC_DATA_EQ:
            if (vpc->cpu.reg[op1] == op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data eq: #%x(%d) == (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;

        case LOGIC_DATA_NEQ:
            if (vpc->cpu.reg[op1] != op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data neq: #%x(%d) != (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;

        case LOGIC_DATA_GE:
            if (vpc->cpu.reg[op1] >= op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data ge: #%x(%d) >= (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;

        case LOGIC_DATA_LE:
            if (vpc->cpu.reg[op1] <= op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data le: #%x(%d) <= (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;

        case LOGIC_DATA_GT:
            if (vpc->cpu.reg[op1] > op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data gt: #%x(%d) > (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;

        case LOGIC_DATA_LT:
            if (vpc->cpu.reg[op1] < op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data lt: #%x(%d) < (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;
    }
}

void stuff_instructions(PC* vpc, word code, word op1, word op2) {
    word sub_code = code & 0x0f;
    dword addr;

    switch (sub_code) {
        case STUFF_IP_SET:
            addr = ((op1 << 16) | (op2));
            vpc->cpu.IP = addr;

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction set: IP = %x\n", addr);
#endif
            break;

        case STUFF_REG_MOV:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg mov: #%x(%d) = #%x(%d)\n", op1, op1, op2, op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op2];
            break;

        case STUFF_DATA_MOV:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data mov: #%x(%d) = (%d)\n", op1, op1, op2);
#endif

            vpc->cpu.reg[op1] = op2;
            break;

        case STUFF_REG_INC:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg inc: #%x(%d)++\n", op2, vpc->cpu.reg[op2]);
#endif

            (vpc->cpu.reg[op2])++;
            break;

        case STUFF_REG_DEC:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg dec: #%x(%d)--\n", op2, vpc->cpu.reg[op2]);
#endif

            (vpc->cpu.reg[op2])--;
            break;
    }
}

void math_instructions(PC* vpc, word code, word op1, word op2) {
    word sub_code = code & 0x0f;

    switch (sub_code) {
        case MATH_REG_ADD:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg add: #%x = #%x(%d) + #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] + vpc->cpu.reg[op2];
            break;

        case MATH_REG_SUB:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg sub: #%x = #%x(%d) - #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] - vpc->cpu.reg[op2];
            break;

        case MATH_REG_MUL:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg mul: #%x = #%x(%d) * #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] * vpc->cpu.reg[op2];
            break;

        case MATH_REG_DIV:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg div: #%x = #%x(%d) / #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] / vpc->cpu.reg[op2];
            break;

        case MATH_REG_MOD:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg mod: #%x = #%x(%d) %c #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], '%', op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] % vpc->cpu.reg[op2];
            break;

        case MATH_DATA_ADD:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data add: #%x = #%x(%d) + (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] + op2;
            break;

        case MATH_DATA_SUB:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data sub: #%x = #%x(%d) - (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] - op2;
            break;                                 
                                                   
        case MATH_DATA_MUL:                                 
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data mul: #%x = #%x(%d) * (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] * op2;
            break;                                 
                                                   
        case MATH_DATA_DIV:                                 
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data div: #%x = #%x(%d) / (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] / op2;
            break;                                 
                                                   
        case MATH_DATA_MOD:                                 
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data mod: #%x = #%x(%d) %c (%d)\n", op1, op1, vpc->cpu.reg[op1], '%', op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] % op2;
            break;
    }
}

void bitwise_instructions(PC* vpc, word code, word op1, word op2) {
    word sub_code = code & 0x0f;

    switch (sub_code) {
        case BITWISE_REG_AND:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg and: AND #%x = #%x(%d) & #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] & vpc->cpu.reg[op2];
            break;

        case BITWISE_REG_OR:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg or: OR #%x = #%x(%d) | #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] | vpc->cpu.reg[op2];
            break;

        case BITWISE_REG_XOR:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg xor: XOR #%x = #%x(%d) ^ #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] ^ vpc->cpu.reg[op2];
            break;

        case BITWISE_REG_INV:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg inv: INV #%x = ~#%x(%d)\n", op1, op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = ~vpc->cpu.reg[op2];
            break;

        case BITWISE_REG_LSH:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg lsh: LSH #%x = #%x(%d) << #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] << vpc->cpu.reg[op2];
            break;

        case BITWISE_REG_RSH:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg rsh: #%x = #%x(%d) >> #%x(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] >> vpc->cpu.reg[op2];
            break;

        case BITWISE_DATA_AND:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data and: #%x = #%x(%d) & (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] & op2;
            break;

        case BITWISE_DATA_OR:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data or: #%x = #%x(%d) | (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] | op2;
            break;

        case BITWISE_DATA_XOR:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data xor: #%x = #%x(%d) ^ (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] ^ op2;
            break;

        case BITWISE_DATA_INV:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data inv: #%x = ~(%d)\n", op1, op2);
#endif

            vpc->cpu.reg[op1] = ~op2;
            break;

        case BITWISE_DATA_LSH:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data lsh: #%x = #%x(%d) << (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] << op2;
            break;

        case BITWISE_DATA_RSH:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data rsh: #%x = #%x(%d) >> (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
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

        default:
#if INST_DEBUG == 1
            fprintf(stderr, "\t[INST_DEBUG]: UNKNOWN section\n");
#endif
    }

#if INST_DEBUG == 1
        fprintf(stderr, "\n");
#endif

    return result;
}
