#include <stdio.h>
#include <stdlib.h>

#include "byte_array.h"
// #include "../common/error_codes.h"
// #include "../common/funcs.h"

ByteArray* create_bytearray() {
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

bool increase_bytearray_size(ByteArray* obj) {
    if ( !obj ) {
        return false;
    }
    /*check if size of array less then minimum and realloc if yes*/
    int status = true;

    if ( (obj->index + 5) < obj->size ) { // min num for check - '+3'
        size_t new_size = obj->size + (obj->size / 2);
        byte* tmp = realloc(obj->array, new_size);

        if ( tmp ) {
            obj->size = new_size;
            obj->array = tmp;

        } else {
            status = false;
        }
    }

    return status;
}

void destroy_bytearray(ByteArray* obj) {
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
