#ifndef __BYTE_ARRAY__
#define __BYTE_ARRAY__

#include <stddef.h>
#include <stdbool.h>

#include "../hardware/basic_types.h"

/**
 * @brief Struct with array of bytes
 */
typedef struct {
    byte* array;  ///< bytes store here
    size_t size;  ///< current size of array
    size_t index; ///< current index to empty cell for write
} ByteArray;

/**
 * @brief Create ByteArray. Alloc memory for struct and array
 * @note Return NULL if can't allocate memory
 * @return obj ByteArray object or NULL
 */
ByteArray* bytearray_create();

/**
 * @brief Increase ByteArray size with formula 'new_size = size + (size / 2)'
 * @note Only realloc errors can be here
 * @param[in, out] obj Valid ByteArray object
 * @return status 
 */
bool bytearray_increase_size(ByteArray* obj);

/**
 * @brief Correctly deallocate ByteArray object
 * @param[in, out] obj Valid ByteArray object
 * @return void
 */
void bytearray_destroy(ByteArray* obj);

#endif /* __BYTE_ARRAY__ */

