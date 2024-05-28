#ifndef __MEM_H__
#define __MEM_H__

#include <stdio.h>

typedef unsigned char MEMORY;

void mem_dump(FILE* fd, void* mem, size_t size);

#endif /* __MEM_H__ */
