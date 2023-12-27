#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

#include <stdlib.h>
#include "../config.h"

void* m_alloc(size_t size);
void* c_alloc(size_t count, size_t size);
void* re_alloc(void* mem, size_t size);
void m_free(void* mem);
void m_destroy();
void full_exit(int code);

#endif
