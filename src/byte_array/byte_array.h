#ifndef __BYTE_ARRAY__
#define __BYTE_ARRAY__

#include <stddef.h>

#include "../hardware/config.h"

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
ByteArray* create_bytearray();

/**
 * @brief Increase ByteArray size with formula 'new_size = size + (size / 2)'
 * @param[in, out] obj Valid ByteArray object
 * @return status 1:OK 0:ERROR
 */
int increase_bytearray_size(ByteArray* obj);

/**
 * @brief Correctly deallocate ByteArray object
 * @param[in, out] obj Valid ByteArray object
 * @return void
 */
void destroy_bytearray(ByteArray* obj);

#endif /* __BYTE_ARRAY__ */

