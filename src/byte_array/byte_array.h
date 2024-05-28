#ifndef __BYTE_ARRAY__
#define __BYTE_ARRAY__

#include <stddef.h>

#include "../hardware/config.h"

typedef struct {
    byte* array;
    size_t size;
    size_t index;
} ByteArray;

ByteArray* create_bytearray();
void init_bytearray(ByteArray* obj);
void increase_bytearray_size(ByteArray* obj);
void destroy_bytearray(ByteArray* obj);

#endif /* __BYTE_ARRAY__ */

