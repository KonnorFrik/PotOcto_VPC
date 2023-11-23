#ifndef __COMMON_FUNCS_H__
#define __COMMON_FUNCS_H__

#include <stdlib.h>
#include <stdio.h>
#include "../config.h"

int is_null(void* mem);
void mem_dump(FILE* fd, byte* arr, size_t size);

#endif
