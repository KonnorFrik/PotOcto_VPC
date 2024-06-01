.RECIPEPREFIX = >
.PHONY = all

CC = gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror -g#-I $(shell pwd)/src

SRC_DIR := src
COMPILER_DIR := $(SRC_DIR)/compiler
COMMON_DIR := $(SRC_DIR)/common
BYTEARRAY_DIR := $(SRC_DIR)/byte_array
STR_FUNCS_DIR := $(SRC_DIR)/str_funcs
HARDWARE_DIR = $(SRC_DIR)/hardware
TRANSLATOR_DIR := $(COMPILER_DIR)/tree_translator
LEXER_DIR = $(COMPILER_DIR)/lexer
TESTS_DIR := $(SRC_DIR)/tests

virt_pc_src := $(wildcard $(HARDWARE_DIR)/*.c) $(COMMON_DIR)/funcs.c
virt_pc_obj := $(virt_pc_src:.c=.o)
virt_pc_target = pototo

cmp_src := $(foreach dir, $(BYTEARRAY_DIR) $(TRANSLATOR_DIR) $(COMPILER_DIR) $(STR_FUNCS_DIR) $(COMMON_DIR) $(LEXER_DIR), $(wildcard $(dir)/*.c))
cmp_obj := $(cmp_src:.c=.o)
compiler_target = pan

test_src := $(foreach dir, $(TESTS_DIR), $(wildcard $(dir)/*.c))
test_obj := $(test_src:.c=.o)
test_target = str_funcs_tests

targets = $(virt_pc_target) $(compiler_target)


all: $(targets) 
tests: $(test_target)

$(virt_pc_target): $(virt_pc_obj)
> $(CC) $(CFLAGS) $^ -o $@

$(compiler_target): $(cmp_obj)
> $(CC) $(CFLAGS) $^ -o $@


$(test_target): $(test_obj)
> $(CC) -g $^ -o $@


clean:
> $(RM) $(virt_pc_obj)
> $(RM) $(cmp_obj)
> $(RM) $(test_obj)

clean_all: clean
> $(RM) $(targets)
> $(RM) $(test_target)

show:
> @echo $(virt_pc_src)
> @echo $(virt_pc_obj)
> @echo ""
> @echo $(cmp_src)
> @echo $(cmp_obj)
> @echo ""
> @echo $(test_src)
> @echo $(test_obj)

