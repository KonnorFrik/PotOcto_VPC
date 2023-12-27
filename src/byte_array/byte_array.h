#ifndef __BYTE_ARRAY_STRUCT__
#define __BYTE_ARRAY_STRUCT__

#include "../config.h"
typedef unsigned long size_t;

typedef struct {
    byte* array;
    size_t size;
    size_t index;
} ByteArray;

#endif

#ifndef __BYTE_ARRAY__
#define __BYTE_ARRAY__

#include "../mem_manager/mem_manager.h" 
#include "../common/funcs.h"
#include <stdio.h>

ByteArray* create_bytearray();
void init_bytearray(ByteArray* obj);
void increase_bytearray_size(ByteArray* obj);
void destroy_bytearray(ByteArray* obj);

#endif

