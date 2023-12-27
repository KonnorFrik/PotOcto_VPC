#ifndef __COMMON_FUNCS_H__
#define __COMMON_FUNCS_H__

#include <stdlib.h>
#include <stdio.h>
#include "../config.h"
#include "../error_codes.h"

int is_null(void* mem);
void mem_dump(FILE* fd, void* mem, size_t size);
void show_error(int code);

#endif
