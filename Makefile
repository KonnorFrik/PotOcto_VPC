.RECIPEPREFIX = >
.PHONY = all

CC = gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror -g

SRC_DIR := src
COMPILER_DIR := $(SRC_DIR)/compiler
COMMON_DIR := $(SRC_DIR)/common
BYTEARRAY_DIR := $(SRC_DIR)/byte_array
STR_FUNCS_DIR := $(SRC_DIR)/str_funcs
TRANSLATOR_DIR := $(SRC_DIR)/tree_translator
TESTS_DIR := $(SRC_DIR)/tests

virt_pc_src := $(SRC_DIR)/pc.c $(SRC_DIR)/instructions.c $(COMMON_DIR)/funcs.c
virt_pc_obj := $(virt_pc_src:.c=.o)

cmp_src := $(foreach dir, $(BYTEARRAY_DIR) $(TRANSLATOR_DIR) $(COMPILER_DIR) $(STR_FUNCS_DIR) $(COMMON_DIR), $(wildcard $(dir)/*.c))
cmp_obj := $(cmp_src:.c=.o)

test_src := $(foreach dir, $(TESTS_DIR), $(wildcard $(dir)/*.c))
test_obj := $(test_src:.c=.o)

targets = virt_pc cmp


all: $(targets) str_funcs_tests
tests: str_funcs_tests

virt_pc: $(virt_pc_obj)
> $(CC) $(CFLAGS) $^ -o $@

cmp: $(cmp_obj)
> $(CC) $(CFLAGS) $^ -o $@


str_funcs_tests: $(test_obj)
> $(CC) -g $^ -o $@


clean:
> $(RM) $(virt_pc_obj)
> $(RM) $(cmp_obj)
> $(RM) $(test_obj)

clean_all: clean
> $(RM) $(targets)
> $(RM) $(tests)

show:
> @echo $(virt_pc_src)
> @echo $(virt_pc_obj)
> @echo ""
> @echo $(cmp_src)
> @echo $(cmp_obj)
> @echo ""
> @echo $(test_src)
> @echo $(test_obj)

