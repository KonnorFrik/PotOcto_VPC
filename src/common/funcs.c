#include <stdio.h>

#include "funcs.h"
#include "error_codes.h"

void show_error(int code) {
    switch ( code ) {
        case MEM_ERROR:
            fprintf(stderr, "Memory Alloc Error\n");
            break;

        case FILE_ERROR:
            fprintf(stderr, "File Error\n");
            break;

        case SYNTAX_ERR:
            fprintf(stderr, "Syntax Error\n");
            break;

        case INVALID_LINE:
            fprintf(stderr, "Invalid line Error\n");
            break;

        case INVALID_WORD:
            fprintf(stderr, "Invalid word Error\n");
            break;

        case TRANSLATE_LINE_ERR:
            fprintf(stderr, "Translation Error\n");
            break;

        default:
            fprintf(stderr, "Error\n");
            break;
    }
    
}
