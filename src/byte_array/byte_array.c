#include "byte_array.h"

ByteArray* create_bytearray() {
    ByteArray* obj = c_alloc(1, sizeof(ByteArray));

    if (!is_null(obj)) {
        init_bytearray(obj);
    }

    return obj;
}

void init_bytearray(ByteArray* obj) {
    obj->size = DEFAULT_SIZE;
    obj->array = c_alloc(obj->size, sizeof(byte));
    obj->index = 0;
}

void increase_bytearray_size(ByteArray* obj) {
    /*check if size of array less then minimum and realloc if yes*/
    if ((obj->index + 5) < obj->size) { // min num for check - '+3'
        size_t new_size = obj->size + (obj->size / 2);
        byte* tmp = re_alloc(obj->array, new_size);

        if (!is_null(tmp)) {
            obj->size = new_size;
            obj->array = tmp;

        } else {
            show_error(MEM_ERROR);
            fprintf(stderr, "increase_bytecode_size: new mem is NULL\n");
            full_exit(MEM_ERROR);
        }
    }
}

void destroy_bytearray(ByteArray* obj) {
    if (!is_null(obj)) {
        if (!is_null(obj->array)) {
            m_free(obj->array);
        }

        m_free(obj);
    }
}
