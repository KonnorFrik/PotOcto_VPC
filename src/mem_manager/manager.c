#include "mem_manager.h"

#define MEM_DEBUG 1

#define BASIC_SIZE 10
#define EXSTAND_SIZE 5

#if MEM_DEBUG == 1
    #include <stdio.h>
#endif

enum {
    ADD = 0,
    REMOVE,
    UPDATE,
    SELF_DESTROY,
};

static void command_add(void*** arr_addr, void* mem, int* size);
static void command_remove(void** memory_storage, int size, void* mem);
static void command_free_inside(void** memory_storage, int size);

static int is_full(void** arr, int size);
static void exstand(void*** arr_addr, int* size_out);
static void memory_manage(void* mem, void* prev, int command);
static int is_null(void* mem);

static int is_null(void* mem) {
    return mem == NULL;
}

static void exstand(void*** arr_addr, int* size_out) {
    int new_size = *size_out + EXSTAND_SIZE;
    void** new_mem = realloc(*arr_addr, new_size * sizeof(void*));

#if MEM_DEBUG == 1
        fprintf(stderr, "\t[MEM_DEBUG]: Exstand arr: %d -> %d\n", *size_out, new_size);
        fprintf(stderr, "\t[MEM_DEBUG]: '%p' -> '%p'\n\n", *arr_addr, new_mem); 
#endif

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

#if MEM_DEBUG == 1
                fprintf(stderr, "\t[MEM_DEBUG]: Add mem: '%p' to pos: %d\n\n", mem, i);
#endif
        }
    }
}

static void command_remove(void** memory_storage, int size, void* mem) {
    for (int i = 0, run = 1; run && i < size; ++i) {
        if (memory_storage[i] == mem && !is_null(mem)) {
            free(mem);
            memory_storage[i] = NULL;
            run = 0;

#if MEM_DEBUG == 1
                fprintf(stderr, "\t[MEM_DEBUG]: Remove mem: '%p' from pos: %d\n\n", mem, i);
#endif
        }
    }
}

static void command_free_inside(void** memory_storage, int size) {
    for (int i = 0; i < size; ++i) {
        if (!is_null(memory_storage[i])) {
            free(memory_storage[i]);

#if MEM_DEBUG == 1
                fprintf(stderr, "\t[MEM_DEBUG]: Remove non-Free addr: #%3d - '%p'\n", i, memory_storage[i]);
#endif

            memory_storage[i] = NULL;
        }
    }

#if MEM_DEBUG == 1
        fprintf(stderr, "\t[MEM_DEBUG]: Free arr: %p\n", memory_storage);
        fprintf(stderr, "\t[MEM_DEBUG]: memory_storage state at exit:\n");
    
        for (int i = 0; i < size; ++i) {
            fprintf(stderr, "\t #%3d - %p\n", i, memory_storage[i]);
        }
    
        fprintf(stderr, "\n");
#endif
}

static void command_update(void** memory_storage, int size, void* mem, void* prev) {
    if (is_null(mem) || is_null(prev)) {
        //error
    }

    for (int i = 0, run = 1; run && i < size; ++i) {
        if (memory_storage[i] == prev) {
            memory_storage[i] = mem;
            run = 0;

#if MEM_DEBUG == 1
                fprintf(stderr, "\t[MEM_DEBUG]: Update pos #%d, '%p' -> '%p'\n", i, prev, mem);
#endif
        }
    }
}

static void memory_manage(void* mem, void* prev, int command) {
    static void** memory_storage = NULL;
    static int size = 0;

    if (is_null(memory_storage)) {
        size = BASIC_SIZE;
        memory_storage = calloc(size, sizeof(void*));

        #if MEM_DEBUG == 1
        fprintf(stderr, "\t[MEM_DEBUG]: Alloc arr '%p' with size: %d position's\n", memory_storage, size);
        #endif
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

    #if MEM_DEBUG == 1
    fprintf(stderr, "\t[MEM_DEBUG]: Malloc %10zu bytes on: '%p'\n", size, mem);
    #endif

    memory_manage(mem, NULL, ADD);
    return mem;
}

void* c_alloc(size_t count, size_t size) {
    void* mem = calloc(count, size);

    #if MEM_DEBUG == 1
    fprintf(stderr, "\t[MEM_DEBUG]: Calloc %10zu bytes(count: %5zu, size: %5zu) on: '%p'\n", count * size, count, size, mem);
    #endif

    memory_manage(mem, NULL, ADD);
    return mem;
}

void* re_alloc(void* mem, size_t size) {
    void* new_mem = realloc(mem, size);

    #if MEM_DEBUG == 1
    fprintf(stderr, "\t[MEM_DEBUG]: Realloc: '%p' to '%p' with size: %zu bytes\n\n", mem, new_mem, size);
    #endif

    memory_manage(new_mem, mem, UPDATE);

    return new_mem;
}

void m_free(void* mem) {
    memory_manage(mem, NULL, REMOVE);
}

void m_destroy() {
    memory_manage(NULL, NULL, SELF_DESTROY);
}

void full_exit(int code) {
    m_destroy();
    exit(code);
}

