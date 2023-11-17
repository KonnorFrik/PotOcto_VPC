#include "mem_manager.h"

static void command_add(void*** arr_addr, void* mem, int* size);
static void command_remove(void** memory_storage, int size, void* mem);
static void command_free_inside(void** memory_storage, int size);

static int is_full(void** arr, int size);
static void exstand(void*** arr_addr, int* size_out);
static void memory(void* mem, void* prev, int command);

int is_null(void* mem) {
    return mem == NULL;
}

static void exstand(void*** arr_addr, int* size_out) {
    int new_size = *size_out + EXSTAND_SIZE;
    void** new_mem = realloc(*arr_addr, new_size * sizeof(void*));

    if (DEBUG) {
        fprintf(stderr, "\t[DEBUG]: Exstand arr: %d -> %d\n", *size_out, new_size);
        fprintf(stderr, "\t[DEBUG]: '%p' -> '%p'\n\n", *arr_addr, new_mem); 
    }

    if (!is_null(new_mem)) {
        //free(*arr_addr);
        *arr_addr = new_mem;

        for (int i = *size_out; i < new_size; ++i ) {
            (*arr_addr)[i] = NULL;
        }

        *size_out = new_size;

    }

}

static int is_full(void** arr, int size) {
    int count = 0;
    int result = 0;

    for (int i = 0; i < size; ++i) {
        if (!is_null(arr[i])) {
            count++;
        }
    }

    if (count == size) {
        result = 1;
    }

    return result;
}

static void command_add(void*** arr_addr, void* mem, int* size) {
    if (is_full(*arr_addr, *size)) {
        exstand(arr_addr, size);
    }

    for (int i = 0, run = 1; run && i < *size; ++i) {
        if (is_null((*arr_addr)[i]) && !is_null(mem) && ((*arr_addr)[i] != mem)) {
            (*arr_addr)[i] = mem;
            run = 0;

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Add mem: '%p' to pos: %d\n\n", mem, i);
            }
        }
    }
}

static void command_remove(void** memory_storage, int size, void* mem) {
    for (int i = 0, run = 1; run && i < size; ++i) {
        if (memory_storage[i] == mem && !is_null(mem)) {
            free(mem);
            memory_storage[i] = NULL;
            run = 0;

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Remove mem: '%p' from pos: %d\n\n", mem, i);
            }
        }
    }
}

static void command_free_inside(void** memory_storage, int size) {
    for (int i = 0; i < size; ++i) {
        if (!is_null(memory_storage[i])) {
            free(memory_storage[i]);

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Remove unFree addr: #%d - '%p'\n", i, memory_storage[i]);
            }

            memory_storage[i] = NULL;
        }
    }

    if (DEBUG) {
        fprintf(stderr, "\t[DEBUG]: Free arr: %p\n", memory_storage);
        fprintf(stderr, "\t[DEBUG]: memory_storage state at exit:\n");
    
        for (int i = 0; i < size; ++i) {
            fprintf(stderr, "\t #%d - %p\n", i, memory_storage[i]);
        }
    
        fprintf(stderr, "\n");
    }
}

static void command_update(void** memory_storage, int size, void* mem, void* prev) {
    if (is_null(mem) || is_null(prev)) {
        //error
    }

    for (int i = 0, run = 1; run && i < size; ++i) {
        if (memory_storage[i] == prev) {
            memory_storage[i] = mem;
            run = 0;

            if (DEBUG) {
                fprintf(stderr, "\t[DEBUG]: Update pos #%d, '%p' -> '%p'\n", i, prev, mem);
            }
        }
    }
}

static void memory(void* mem, void* prev, int command) {
    static void** memory_storage = NULL;
    static int size = 0;

    if (is_null(memory_storage)) {
        memory_storage = calloc(BASIC_SIZE, sizeof(void*));
        size = BASIC_SIZE;

        if (DEBUG) {
            fprintf(stderr, "\t[DEBUG]: Alloc arr '%p' with size: %d bytes\n", memory_storage, size);
        }
    }

    switch (command) {
        case ADD:
            command_add(&memory_storage, mem, &size);
            break;
            
        case REMOVE:
            command_remove(memory_storage, size, mem);
            break;

        case UPDATE:
            command_update(memory_storage, size, mem, prev); 
            break;

        case SELF_DESTROY:
            command_free_inside(memory_storage, size);

            if (!is_null(memory_storage)) {
                free(memory_storage);
                memory_storage = NULL;
            }

            break;
    }
}

void* m_alloc(size_t size) {
    void* mem = malloc(size);

    if (DEBUG) {
        fprintf(stderr, "\t[DEBUG]: Malloc %zu bytes on: '%p'\n", size, mem);
    }

    memory(mem, NULL, ADD);
    return mem;
}

void* c_alloc(size_t count, size_t size) {
    void* mem = calloc(count, size);

    if (DEBUG) {
        fprintf(stderr, "\t[DEBUG]: Calloc %zu bytes(count: %zu, size: %zu) on: '%p'\n", count * size, count, size, mem);
    }

    memory(mem, NULL, ADD);
    return mem;
}

void* re_alloc(void* mem, size_t size) {
    void* new_mem = realloc(mem, size);

    if (DEBUG) {
        fprintf(stderr, "\t[DEBUG]: Realloc: '%p' to '%p' with size: %zu bytes\n\n", mem, new_mem, size);
    }

    memory(new_mem, mem, UPDATE);

    return new_mem;
}

void m_free(void* mem) {
    memory(mem, NULL, REMOVE);
}

void m_destroy() {
    memory(NULL, NULL, SELF_DESTROY);
}
