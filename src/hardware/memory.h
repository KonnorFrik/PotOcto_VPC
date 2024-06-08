#ifndef __MEM_H__
#define __MEM_H__

#include <stdio.h>

typedef unsigned char MEMORY;

/**
 * @brief Print given array of bytes in given FILE in hex format
 * @param[in] fd FILE object for print in
 * @param[in] mem Array for print it
 * @param[in] size Size of given array
 * @return void
 */
void mem_dump(FILE* fd, void* mem, size_t size);

#endif /* __MEM_H__ */
