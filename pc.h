#ifndef __VIRT_PC_H__
#define __VIRT_PC_H__

#include "mem.h"
#include "cpu.h"

#define MEM_SIZE (0xffff + 1)

typedef unsigned char word;
typedef unsigned short dword;

typedef struct {
    CPU cpu;
    MEMORY memory [MEM_SIZE];
} PC;

enum ERROR_CODES {
    OK = 0,
    WRONG_ARGS,
    FILE_ERROR,
    MAP_ERROR,
};

#endif
