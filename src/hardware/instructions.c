#include <stdio.h>

#include "instructions_executors.h"
#include "instructions_map.h"

#define INST_DEBUG 1

void memory_instructions(PC* vpc, word code, word op1, word op2) {
    word sub_code = code & 0x0f;
    dword addr = 0;

    switch ( sub_code ) {
        case MEM_SET:
            addr = ((op1 << 8) | (op2));
            vpc->cpu.MP = addr;

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction set: MP = %x\n", addr);
#endif
            break;
        
        case MEM_READ:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction read: #%d(%d) = Mem[%x](%d)\n", op2, vpc->cpu.reg[op2], vpc->cpu.MP, vpc->memory[vpc->cpu.MP]);
#endif

            vpc->cpu.reg[op2] = vpc->memory[vpc->cpu.MP];
            break;
        
        case MEM_WRITE:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction write: Mem[%x] = #%d(%d)\n", vpc->cpu.MP, op2, vpc->cpu.reg[op2]);
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
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg eq: #%d(%d) == #%d(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case LOGIC_REG_NEQ:
            if (vpc->cpu.reg[op1] != vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg neq: #%d(%d) != #%d(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case LOGIC_REG_GE:
            if (vpc->cpu.reg[op1] >= vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg ge: #%d(%d) >= #%d(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case LOGIC_REG_LE:
            if (vpc->cpu.reg[op1] <= vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg le: #%d(%d) <= #%d(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case LOGIC_REG_GT:
            if (vpc->cpu.reg[op1] > vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg gt: #%d(%d) > #%d(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case LOGIC_REG_LT:
            if (vpc->cpu.reg[op1] < vpc->cpu.reg[op2]) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg lt: #%d(%d) < #%d(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif
            break;

        case LOGIC_DATA_EQ:
            if (vpc->cpu.reg[op1] == op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data eq: #%d(%d) == (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;

        case LOGIC_DATA_NEQ:
            if (vpc->cpu.reg[op1] != op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data neq: #%d(%d) != (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;

        case LOGIC_DATA_GE:
            if (vpc->cpu.reg[op1] >= op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data ge: #%d(%d) >= (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;

        case LOGIC_DATA_LE:
            if (vpc->cpu.reg[op1] <= op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data le: #%d(%d) <= (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;

        case LOGIC_DATA_GT:
            if (vpc->cpu.reg[op1] > op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data gt: #%d(%d) > (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif
            break;

        case LOGIC_DATA_LT:
            if (vpc->cpu.reg[op1] < op2) {
                vpc->cpu.IP += 3;
            }

#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data lt: #%d(%d) < (%d)\n", op1, vpc->cpu.reg[op1], op2);
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
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg mov: #%d(%d) = #%d(%d)\n", op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op2];
            break;

        case STUFF_DATA_MOV:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data mov: #%d(%d) = (%d)\n", op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = op2;
            break;

        case STUFF_REG_INC:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg inc: #%d(%d)++\n", op2, vpc->cpu.reg[op2]);
#endif

            (vpc->cpu.reg[op2])++;
            break;

        case STUFF_REG_DEC:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg dec: #%d(%d)--\n", op2, vpc->cpu.reg[op2]);
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
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg add: #%d = #%d(%d) + #%d(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] + vpc->cpu.reg[op2];
            break;

        case MATH_REG_SUB:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg sub: #%d = #%d(%d) - #%d(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] - vpc->cpu.reg[op2];
            break;

        case MATH_REG_MUL:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg mul: #%d = #%d(%d) * #%d(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] * vpc->cpu.reg[op2];
            break;

        case MATH_REG_DIV:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg div: #%d = #%d(%d) / #%d(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] / vpc->cpu.reg[op2];
            break;

        case MATH_REG_MOD:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg mod: #%d = #%d(%d) %c #%d(%d)\n", op1, op1, vpc->cpu.reg[op1], '%', op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] % vpc->cpu.reg[op2];
            break;

        case MATH_DATA_ADD:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data add: #%d = #%d(%d) + (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] + op2;
            break;

        case MATH_DATA_SUB:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data sub: #%d = #%d(%d) - (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] - op2;
            break;                                 
                                                   
        case MATH_DATA_MUL:                                 
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data mul: #%d = #%d(%d) * (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] * op2;
            break;                                 
                                                   
        case MATH_DATA_DIV:                                 
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data div: #%d = #%d(%d) / (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] / op2;
            break;                                 
                                                   
        case MATH_DATA_MOD:                                 
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data mod: #%d = #%d(%d) %c (%d)\n", op1, op1, vpc->cpu.reg[op1], '%', op2);
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
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg and: AND #%d = #%d(%d) & #%d(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] & vpc->cpu.reg[op2];
            break;

        case BITWISE_REG_OR:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg or: OR #%d = #%d(%d) | #%d(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] | vpc->cpu.reg[op2];
            break;

        case BITWISE_REG_XOR:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg xor: XOR #%d = #%d(%d) ^ #%d(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] ^ vpc->cpu.reg[op2];
            break;

        case BITWISE_REG_INV:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg inv: INV #%d = ~#%d(%d)\n", op1, op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = ~vpc->cpu.reg[op2];
            break;

        case BITWISE_REG_LSH:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg lsh: LSH #%d = #%d(%d) << #%d(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] << vpc->cpu.reg[op2];
            break;

        case BITWISE_REG_RSH:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction reg rsh: #%d = #%d(%d) >> #%d(%d)\n", op1, op1, vpc->cpu.reg[op1], op2, vpc->cpu.reg[op2]);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] >> vpc->cpu.reg[op2];
            break;

        case BITWISE_DATA_AND:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data and: #%d = #%d(%d) & (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] & op2;
            break;

        case BITWISE_DATA_OR:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data or: #%d = #%d(%d) | (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] | op2;
            break;

        case BITWISE_DATA_XOR:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data xor: #%d = #%d(%d) ^ (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] ^ op2;
            break;

        case BITWISE_DATA_INV:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data inv: #%d = ~(%d)\n", op1, op2);
#endif

            vpc->cpu.reg[op1] = ~op2;
            break;

        case BITWISE_DATA_LSH:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data lsh: #%d = #%d(%d) << (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
#endif

            vpc->cpu.reg[op1] = vpc->cpu.reg[op1] << op2;
            break;

        case BITWISE_DATA_RSH:
#if INST_DEBUG == 1
                fprintf(stderr, "\t[INST_DEBUG]: Instruction data rsh: #%d = #%d(%d) >> (%d)\n", op1, op1, vpc->cpu.reg[op1], op2);
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
