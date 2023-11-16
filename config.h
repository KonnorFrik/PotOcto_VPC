#ifndef __CONFIG_H__
#define __CONFIG_H__

typedef unsigned char word;
typedef unsigned short dword;

enum ERROR_CODES {
    OK = 0,
    WRONG_ARGS,
    FILE_ERROR,
    MAP_ERROR,
};

#endif
