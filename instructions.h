#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "pc.h"

#define DEBUG 1
#if DEBUG == 1
    #include <stdio.h>
#endif

void memory_instructions(PC* vpc, word code, word op1, word op2);
void logic_instructions(PC* vpc, word code, word op1, word op2);
void jmp_instructions(PC* vpc, word code, word op1, word op2);
void math_instructions(PC* vpc, word code, word op1, word op2);
void bitwise_instructions(PC* vpc, word code, word op1, word op2);
int do_instruction(PC* vpc, word code, word op1, word op2);

#endif
