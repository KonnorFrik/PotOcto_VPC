/*
* @file Each *_instructions function run instruction in given PC object
*/

#ifndef __INSTRUCTIONS_EXECUTORS_H__
#define __INSTRUCTIONS_EXECUTORS_H__

#include "pc.h"
#include "basic_types.h"

void memory_instructions(PC* vpc, word code, word op1, word op2);
void logic_instructions(PC* vpc, word code, word op1, word op2);
void jmp_instructions(PC* vpc, word code, word op1, word op2);
void math_instructions(PC* vpc, word code, word op1, word op2);
void bitwise_instructions(PC* vpc, word code, word op1, word op2);

/**
 * @brief Determine type of instruction and run matched executor
 * @note Return 0 if 'hlt' found
 * @param[in, out] vpc Valid PC object
 * @param[in]      code Instruction type
 * @param[in]      op1 Instruction 1st operand
 * @param[in]      op2 Instruction 2st operand
 * @return status - Can pc run next instruction or not
 */
int do_instruction(PC* vpc, word code, word op1, word op2);

#endif /* __INSTRUCTIONS_EXECUTORS_H__ */

