#ifndef __VIRT_PC_H__
#define __VIRT_PC_H__

#include "hardware/memory.h"
#include "hardware/cpu.h"
#include "hardware/config.h"
#include "common/funcs.h"

#define MEM_SIZE (0xffff + 1)

typedef struct {
    CPU cpu;
    MEMORY memory [MEM_SIZE];
} PC;

#endif
