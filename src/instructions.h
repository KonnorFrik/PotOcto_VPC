#ifndef __INSTRUCTIONS_PARSE_H__
#define __INSTRUCTIONS_PARSE_H__

#include "pc.h"
#include "config.h"

/**
* @brief Each *_instructions function RUN instruction in PC object
*/
void memory_instructions(PC* vpc, word code, word op1, word op2);
void logic_instructions(PC* vpc, word code, word op1, word op2);
void jmp_instructions(PC* vpc, word code, word op1, word op2);
void math_instructions(PC* vpc, word code, word op1, word op2);
void bitwise_instructions(PC* vpc, word code, word op1, word op2);
int do_instruction(PC* vpc, word code, word op1, word op2);

#endif

#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

enum INSTRUCTIONS {
    MEMORY_SECTION = 0x10,
    LOGIC_SECTION = 0x20,
    STUFF_SECTION = 0x30,
    MATH_SECTION = 0xe0,
    BITWISE_SECTION = 0xf0,
};

#endif
