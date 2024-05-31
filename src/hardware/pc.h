#ifndef __VIRT_PC_H__
#define __VIRT_PC_H__

#include "memory.h"
#include "cpu.h"
#include "basic_types.h"
#include "../common/funcs.h"

#define MEM_SIZE (0xffff + 1)

/**
 * @brief Struct with VirtPC configuration
 */
typedef struct {
    CPU cpu;
    MEMORY memory [MEM_SIZE];
} PC;

// TODO: write a description and mb exaple for each instruction in readme

#endif /* __VIRT_PC_H__ */
