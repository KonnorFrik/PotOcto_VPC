#ifndef __CPU_H__
#define __CPU_H__

#define REG_COUNT 256

typedef struct {
    unsigned short IP;
    unsigned short MP;
    unsigned char reg[REG_COUNT];
} CPU;

#endif
