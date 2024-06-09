#ifndef __ERROR_CODES__
#define __ERROR_CODES__

/**
 * @brief Error codes for all programm
 */
enum ERROR_CODES {
    ERROR = -1, ///< Unknown error
    OK = 0,     ///< No error
    WRONG_ARGS, // 1
    FILE_ERROR, // 2
    MAP_ERROR,
    MEM_ERROR,  // 4
    SYNTAX_ERR,
    INVALID_LINE,
    INVALID_WORD,
    TRANSLATE_LINE_ERR,
};

#endif /* __ERROR_CODES__ */
