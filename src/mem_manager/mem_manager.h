#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

#include <stdlib.h>
#include <stdio.h>

#include "../config.h"

#define BASIC_SIZE 10
#define EXSTAND_SIZE 5

typedef enum {
    ADD = 0,
    REMOVE,
    UPDATE,
    SELF_DESTROY,
} COMMANDS;

void* m_alloc(size_t size);
void* c_alloc(size_t count, size_t size);
void* re_alloc(void* mem, size_t size);
void m_free(void* mem);
void m_destroy();

int is_null(void* mem);


#endif
