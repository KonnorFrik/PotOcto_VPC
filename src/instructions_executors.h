#ifndef __INSTRUCTIONS_EXECUTORS_H__
#define __INSTRUCTIONS_EXECUTORS_H__

#include "pc.h"
#include "hardware/config.h"

/*
* @brief Each *_instructions function RUN instruction in PC object
*/
void memory_instructions(PC* vpc, word code, word op1, word op2);
void logic_instructions(PC* vpc, word code, word op1, word op2);
void jmp_instructions(PC* vpc, word code, word op1, word op2);
void math_instructions(PC* vpc, word code, word op1, word op2);
void bitwise_instructions(PC* vpc, word code, word op1, word op2);
int do_instruction(PC* vpc, word code, word op1, word op2);

#endif /* __INSTRUCTIONS_EXECUTORS_H__ */

