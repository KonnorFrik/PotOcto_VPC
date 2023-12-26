+====================================================+
| Virtual PC with own CPU                            |
|                     Instructions set               |
|                     Compiler                       |
|                     Virtual machine                |
+====================================================+
|                 * Registers *                      |
|                   IP   - 16 bit                    |
|                   MP   - 16 bit                    |
|                   0-ff -  8 bit                    |
+====================================================+
|                * Instructions *                    |
|              XX, YY - register name                |
|              DD - constant data 1 byte             |
|              AA - address                          |
|----------------------------------------------------|
|             * Work with memory *                   |
|   10AAAA - set Mem Pointer with addr 'A..A'        |
|   1100XX - XX = mem[MP] // read mem[MP] to rXX     |
|   1200XX - mem[MP] = XX // write rXX to mem[MP]    |
|   130000 - increase Mem Pointer // inc             |
|   140000 - decrease Mem Pointer                    |
|----------------------------------------------------|
|                  * Logic *                         |
|       20YYXX - skip next if YY == XX               |
|       21YYXX - skip next if YY != XX               |
|       22YYXX - skip next if YY >= XX               |
|       23YYXX - skip next if YY <= XX               |
|       24YYXX - skip next if YY > XX                |
|       25YYXX - skip next if YY < XX                |
|                                                    |
|       26YYDD - skip next if YY == DD               |
|       27YYDD - skip next if YY != DD               |
|       28YYDD - skip next if YY >= DD               |
|       29YYDD - skip next if YY <= DD               |
|       2AYYDD - skip next if YY > DD                |
|       2BYYDD - skip next if YY < DD                |
|----------------------------------------------------|
|                  * Stuff *                         |
|       30AAAA - jmp to AAAA                         |
|       31YYXX - mov XX to YY                        |
|       32YYDD - mov DD to YY                        |
|       3300XX - increase XX // inc rXX              |
|       3400XX - decrease XX                         |
|----------------------------------------------------|
|                  * Math *                          |
|       E0YYXX - YY = YY + XX                        |
|       E1YYXX - YY = YY - XX                        |
|       E2YYXX - YY = YY * XX                        |
|       E3YYXX - YY = YY / XX                        |
|       E4YYXX - YY = YY % XX                        |
|                                                    |
|       E5YYDD - YY = YY + DD                        |
|       E6YYDD - YY = YY - DD                        |
|       E7YYDD - YY = YY * DD                        |
|       E8YYDD - YY = YY / DD                        |
|       E9YYDD - YY = YY % DD                        |
|----------------------------------------------------|
|                  * Bitwise *                       |
|       F0YYXX - bitwise YY AND XX, result in YY     |
|       F1YYXX - bitwise YY OR  XX, result in YY     |
|       F2YYXX - bitwise YY XOR XX, result in YY     |
|       F3YYXX - bitwise invert XX, result in YY     |
|       F4YYXX - bitwise lshift YY on count in XX    |
|       F5YYXX - bitwise rshift YY on count in XX    |
|                                                    |
|       F6YYDD - bitwise YY AND DD, result in YY     |
|       F7YYDD - bitwise YY OR  DD, result in YY     |
|       F8YYDD - bitwise YY XOR DD, result in YY     |
|       F9YYDD - bitwise invert DD, result in YY     |
|       FAYYDD - bitwise lshift YY on count DD       |
|       FBYYDD - bitwise rshift YY on count DD       |
|----------------------------------------------------|
|                  * Other *                         |
|       FFFFFF - Halt                                |
+====================================================+
|                  * Syntax *                        |
| [KEYWORD] [[OP_ACCESS] [DEST]] [[OP_ACCESS] [SRC]] |
|----------------------------------------------------|
| KEYWORD:                                           |
|       1 mov                                        |
|       2 jmp                                        |
|       3 inc                                        |
|       4 dec                                        |
|                                                    |
|       5 eq                                         |
|       6 ne                                         |
|       7 ge                                         |
|       8 le                                         |
|       9 gt                                         |
|       10 lt                                        |
|                                                    |
|       11 add                                       |
|       12 sub                                       |
|       13 mul                                       |
|       14 div                                       |
|       15 mod                                       |
|                                                    |
|       16 and                                       |
|       17 or                                        |
|       18 xor                                       |
|       19 inv                                       |
|       20 lsh                                       |
|       21 rsh                                       |
|                                                    |
|       22 read                                      |
|       23 write                                     |
|       24 set                                       |
|                                                    |
|       25 hlt                                       |
|                                                    |
|   OP_ACCESS:                                       |
|       r - access to register                       |
|                                                    |
|   DST = SRC = [NUM]:                               |
|       hex num,                                     |
|       decimal num,                                 |
+====================================================+

