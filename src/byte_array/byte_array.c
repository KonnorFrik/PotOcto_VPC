#include <stdio.h>
#include <stdlib.h>

#include "byte_array.h"
#include "../common/error_codes.h"

#define BTR_DEBUG 0

ByteArray* bytearray_create() {
    ByteArray* obj = calloc(1, sizeof(ByteArray));

    if ( obj ) {
        obj->size = DEFAULT_SIZE;
        obj->index = 0;
        obj->array = calloc(obj->size, sizeof(byte));

        if ( !obj->array ) {
            free(obj);
            obj = NULL;
        }
    }

    return obj;
}

int bytearray_increase_size(ByteArray* obj) {
    if ( !obj ) {
        return WRONG_ARGS;
    }
    /*check if size of array less then minimum and realloc if yes*/
    int status = OK;

    if ( (obj->index + 3) >= obj->size ) { // min num for check - '+3'
        size_t new_size = obj->size + (obj->size / 2);

#if BTR_DEBUG == 1
        fprintf(stderr, "[BTR_DEBUG]: Increase: old size: %lu , new size: %lu\n", obj->size, new_size);
#endif
        byte* tmp = realloc(obj->array, new_size);

#if BTR_DEBUG == 1
        fprintf(stderr, "[BTR_DEBUG]: Increase: new address: %p\n", (void*)tmp);
#endif

        if ( tmp ) {
            obj->size = new_size;
            obj->array = tmp;

        } else {
            status = MEM_ERROR;
        }
    }

    return status;
}

void bytearray_destroy(ByteArray* obj) {
    if ( !obj ) {
        return;
    }

    if ( obj ) {
        if ( obj->array ) {
            free(obj->array);
        }

        free(obj);
    }
}
