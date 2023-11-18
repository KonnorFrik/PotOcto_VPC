#ifndef __CONFIG_H__
#define __CONFIG_H__

#define DEBUG 1

typedef unsigned char word;
typedef unsigned short dword;

enum ERROR_CODES {
    OK = 0,
    WRONG_ARGS,
    FILE_ERROR,
    MAP_ERROR,
    MEM_ERROR,
    ERROR = -1,
};

#endif
