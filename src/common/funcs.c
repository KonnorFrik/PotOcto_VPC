#include <stdio.h>

#include "funcs.h"
#include "error_codes.h"

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
