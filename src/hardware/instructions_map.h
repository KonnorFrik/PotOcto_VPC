#ifndef __INSTRUCTIONS_MAP_H__
#define __INSTRUCTIONS_MAP_H__

enum INSTRUCTIONS {
    MEMORY_SECTION = 0x10,
    LOGIC_SECTION = 0x20,
    STUFF_SECTION = 0x30,
    MATH_SECTION = 0xe0,
    BITWISE_SECTION = 0xf0,
};

enum INST_MEMORY_SECTION {
    MEM_SET = 0x00,
    MEM_READ = 0x01,
    MEM_WRITE = 0x02,
    MEM_INC = 0x03,
    MEM_DEC = 0x04,
};

enum INST_LOGIC_SECTION {
    LOGIC_REG_EQ = 0x00,
    LOGIC_REG_NEQ = 0x01,
    LOGIC_REG_GE = 0x02,
    LOGIC_REG_LE = 0x03,
    LOGIC_REG_GT = 0x04,
    LOGIC_REG_LT = 0x05,

    LOGIC_DATA_EQ = 0x06,
    LOGIC_DATA_NEQ = 0x07,
    LOGIC_DATA_GE = 0x08,
    LOGIC_DATA_LE = 0x09,
    LOGIC_DATA_GT = 0x0a,
    LOGIC_DATA_LT = 0x0b,
};

enum INST_STUFF_SECTION {
    STUFF_IP_SET = 0x00,
    STUFF_REG_MOV = 0x01,
    STUFF_DATA_MOV = 0x02,
    STUFF_REG_INC = 0x03,
    STUFF_REG_DEC = 0x04,
};

enum INST_MATH_SECTION {
    MATH_REG_ADD = 0x00,
    MATH_REG_SUB = 0x01,
    MATH_REG_MUL = 0x02,
    MATH_REG_DIV = 0x03,
    MATH_REG_MOD = 0x04,

    MATH_DATA_ADD = 0x05,
    MATH_DATA_SUB = 0x06,
    MATH_DATA_MUL = 0x07,
    MATH_DATA_DIV = 0x08,
    MATH_DATA_MOD = 0x09,
};

enum INST_BITWISE_SECTION {
    BITWISE_REG_AND = 0x00,
    BITWISE_REG_OR = 0x01,
    BITWISE_REG_XOR = 0x02,
    BITWISE_REG_INV = 0x03,
    BITWISE_REG_LSH = 0x04,
    BITWISE_REG_RSH = 0x05,

    BITWISE_DATA_AND = 0x06,
    BITWISE_DATA_OR = 0x07,
    BITWISE_DATA_XOR = 0x08,
    BITWISE_DATA_INV = 0x09,
    BITWISE_DATA_LSH = 0x0a,
    BITWISE_DATA_RSH = 0x0b,
};

#endif /* __INSTRUCTIONS_MAP_H__ */
