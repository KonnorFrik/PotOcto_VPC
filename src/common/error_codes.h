#ifndef __ERROR_CODES__
#define __ERROR_CODES__

/**
 * @brief Error codes for all programm
 */
enum {
    ERROR = -1, ///< Unknown error
    OK = 0,     ///< No error
    WRONG_ARGS,
    FILE_ERROR,
    MAP_ERROR,
    MEM_ERROR,
    SYNTAX_ERR,
    INVALID_LINE,
    INVALID_WORD,
    TRANSLATE_LINE_ERR,
};

#endif /* __ERROR_CODES__ */
