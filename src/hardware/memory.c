#include "memory.h"
#include "config.h"

static char decode_symb(byte symb);
static void fprintf_decoded_row(FILE* fd, unsigned char* arr, unsigned short start_addr);

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
