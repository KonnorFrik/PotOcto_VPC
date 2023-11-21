#include "funcs.h"

int is_null(void* mem) {
    return mem == NULL;
}

// max4 16 per row  16 per row
// addr bytes       decoded
// FFFF 00 00 .. 00 ....

static char decode_symb(byte symb) {
    char result = '.';

    if (symb > 31 && symb < 127) {
        result = (char)symb;
    }

    return result;
}

static void fprintf_decoded_row(FILE* fd, byte* arr, dword start_addr) {
    for (dword addr = start_addr - 16; addr < start_addr; ++addr) {
        fprintf(fd, "%c", decode_symb(arr[addr]));
    }
}

void mem_dump(FILE* fd, byte* arr, size_t size) {
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

