#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../str_funcs/str_funcs.h"
#include "../str_funcs/str_funcs.c"

void startswith_test() {
    char str[100], format[100];
    int exp, res;

    strcpy(str, "hello world");
    strcpy(format, "hel");
    exp = 1;
    res = startswith((const char*)str, (const char*)format);
    printf("str: '%s' | format: '%s' | exp: %d | res: %d\n", str, format, exp, res);
    assert(exp == res);

    strcpy(str, "hello world");
    strcpy(format, "ello");
    exp = 0;
    res = startswith((const char*)str, (const char*)format);
    printf("str: '%s' | format: '%s' | exp: %d | res: %d\n", str, format, exp, res);
    assert(exp == res);

    //
    //strcpy(str, "");
    //strcpy(format, "hello");
    //exp = 0;
    //res = startswith((const char*)str, (const char*)format);
    //printf("str: '%s' | format: '%s' | exp: %d | res: %d\n", str, format, exp, res);
    //assert(exp == res);

    strcpy(str, "hello world");
    strcpy(format, "");
    exp = 1;
    res = startswith((const char*)str, (const char*)format);
    printf("str: '%s' | format: '%s' | exp: %d | res: %d\n", str, format, exp, res);
    assert(exp == res);

    strcpy(str, "0x23f");
    strcpy(format, "0x");
    exp = 1;
    res = startswith((const char*)str, (const char*)format);
    printf("str: '%s' | format: '%s' | exp: %d | res: %d\n", str, format, exp, res);
    assert(exp == res);

    strcpy(str, "0b101");
    strcpy(format, "b01");
    exp = 0;
    res = startswith((const char*)str, (const char*)format);
    printf("str: '%s' | format: '%s' | exp: %d | res: %d\n", str, format, exp, res);
    assert(exp == res);

    printf("DONE\n");
}

void is_str_dec_test() {
    char str[100];
    int exp, res;

    strcpy(str, "123422346657");
    exp = 1;
    res = is_str_dec((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "hello world");
    exp = 0;
    res = is_str_dec((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "12g3422346657");
    exp = 0;
    res = is_str_dec((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "0x1234");
    exp = 0;
    res = is_str_dec((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "0b1234");
    exp = 0;
    res = is_str_dec((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    printf("DONE\n");
}

void is_str_hex_test() {
    char str[100];
    int exp, res;

    strcpy(str, "123422346657");
    exp = 0;
    res = is_str_hex((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "hello world");
    exp = 0;
    res = is_str_hex((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "12g3422346657");
    exp = 0;
    res = is_str_hex((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "0x1234");
    exp = 1;
    res = is_str_hex((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "0b1234");
    exp = 0;
    res = is_str_hex((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "0x1234567890abcdef");
    exp = 1;
    res = is_str_hex((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    printf("DONE\n");
}

void is_str_bin_test() {
    char str[100];
    int exp, res;

    strcpy(str, "123422346657");
    exp = 0;
    res = is_str_bin((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "hello world");
    exp = 0;
    res = is_str_bin((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "12g3422346657");
    exp = 0;
    res = is_str_bin((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "0x1234febcd");
    exp = 0;
    res = is_str_bin((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "0x10101");
    exp = 0;
    res = is_str_bin((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "0b0010101");
    exp = 1;
    res = is_str_bin((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);
    
    printf("DONE\n");
}

void string_type_test() {
    char str[100];
    int exp, res;

    strcpy(str, "hello world");
    exp = INVALID;
    res = string_type((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "0123456789");
    exp = DEC;
    res = string_type((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "01234f56789");
    exp = INVALID;
    res = string_type((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "0x0123456789abcdef");
    exp = HEX;
    res = string_type((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "0b0101010");
    exp = BIN;
    res = string_type((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    strcpy(str, "0x0101010");
    exp = HEX;
    res = string_type((const char*)str);
    printf("str: '%s' | exp: %d | res: %d\n", str, exp, res);
    assert(exp == res);

    printf("DONE\n");
}

int main() {
    printf("TEST: str_funcs\n");
    printf("Startswith:\n"); 
    startswith_test();

    printf("\nIs str decimal:\n"); 
    is_str_dec_test();

    printf("\nIs str hexadecimal:\n"); 
    is_str_hex_test();

    printf("\nIs str bin:\n"); 
    is_str_bin_test();

    printf("\nString type:\n"); 
    string_type_test();

    printf("SUCCESS\n");
    return 0;
}
