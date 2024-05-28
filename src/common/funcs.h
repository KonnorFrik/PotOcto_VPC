#ifndef __COMMON_FUNCS_H__
#define __COMMON_FUNCS_H__

#include <stdio.h>

void mem_dump(FILE* fd, void* mem, size_t size);
void show_error(int code);

#endif
