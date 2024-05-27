#include <stdlib.h>

#include "../config.h"
#include "funcs.h"
#include "../error_codes.h"

static char decode_symb(byte symb);
static void fprintf_decoded_row(FILE* fd, unsigned char* arr, unsigned short start_addr);

int is_null(void* mem) {
    return mem == NULL;
}

// max4 | 16 per row  | 16 symb's per row
// addr | bytes       | decoded symbol's
// FFFF | 00 00 .. 00 | ....

static char decode_symb(byte symb) {
    char result = '.';

    // mb isascii will be better
    // need check only is symb printable as normal symb
    if (symb > 31 && symb < 127) {
        result = (char)symb;
    }

    return result;
}

static void fprintf_decoded_row(FILE* fd, unsigned char* arr, unsigned short start_addr) {
    for (dword addr = start_addr - 16; addr < start_addr; ++addr) {
        fprintf(fd, "%c", decode_symb(arr[addr]));
    }
}

void mem_dump(FILE* fd, void* mem, size_t size) {
    unsigned char* arr = (unsigned char*)mem;
    fprintf(fd, "       "); // 4 space for addr and 1 for space

    for (int i = 0; i < 16; ++i) {
        fprintf(fd, "0%X  ", i);
    }

    fprintf(fd, "\n");
    dword address = 0;

    while (address < size) {
        byte current_symb = arr[address];

        if ((address % 16) == 0) {
            fprintf_decoded_row(fd, arr, address);
            fprintf(fd, "\n");
            fprintf(fd, "%4X:  ", address);
        }

        if (current_symb < 0x10) {
            fprintf(fd, "0");
        }

        fprintf(fd, "%X  ", current_symb);
        address++;
    }

    fprintf_decoded_row(fd, arr, address);
    fprintf(fd, "\n");
}

void show_error(int code) {
    if (code & MEM_ERROR) {
        fprintf(stderr, "Memory Alloc Error\n");

    } else if (code & FILE_ERROR) {
        fprintf(stderr, "File Error\n");

    } else if (code & SYNTAX_ERR) {
        fprintf(stderr, "Syntax Error\n");

    } else if (code & INVALID_LINE) {
        fprintf(stderr, "Invalid line Error\n");

    } else if (code & INVALID_WORD) {
        fprintf(stderr, "Invalid word Error\n");

    } else if (code & TRANSLATE_LINE_ERR) {
        fprintf(stderr, "Translation Error\n");

    } else {
        fprintf(stderr, "Error\n");
    }
    
}
