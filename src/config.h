#ifndef __CONFIG_H__
#define __CONFIG_H__

#define OK 0

typedef unsigned char byte;
typedef unsigned char word;
typedef unsigned short dword;

enum ERROR_CODES {
    ERROR = -1,
    WRONG_ARGS = 1 << 0,
    FILE_ERROR = 1 << 1,
    MAP_ERROR = 1 << 2,
    MEM_ERROR = 1 << 3,
};

#endif
