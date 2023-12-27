#ifndef __ERROR_CODES__
#define __ERROR_CODES__
enum {
    ERROR = -1,
    OK = 0,
    WRONG_ARGS = 1 << 0,
    FILE_ERROR = 1 << 1,
    MAP_ERROR = 1 << 2,
    MEM_ERROR = 1 << 3,

    SYNTAX = 1 << 10,
    INVALID_LINE = 1 << 11,
    INVALID_WORD = 1 << 12,
    TRANSLATE_LINE = 1 << 13,
};

#endif
