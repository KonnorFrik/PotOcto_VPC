.RECIPEPREFIX = >
.PHONY = all

CC = gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror -g#-I $(shell pwd)/src
LDFLAGS = -lcheck
TEST_FLAGS = `pkg-config --cflags --libs check` -O0
GCOV_FLAGS = -lgcov --coverage

REPORT = REPORT.html
GCOV_DIR = report
LOCAL_GCOVR = gcovr

SRC_DIR := src
COMPILER_DIR := $(SRC_DIR)/compiler
COMMON_DIR := $(SRC_DIR)/common
BYTEARRAY_DIR := $(SRC_DIR)/byte_array
STR_FUNCS_DIR := $(SRC_DIR)/str_funcs
HARDWARE_DIR = $(SRC_DIR)/hardware
TRANSLATOR_DIR := $(COMPILER_DIR)/tree_translator
LEXER_DIR = $(COMPILER_DIR)/lexer

TESTS_DIR := $(SRC_DIR)/tests
TESTS_LEXER_DIR = $(TESTS_DIR)/lexer_tests

LEXER_SRC = $(wildcard $(LEXER_DIR)/*.c)

virt_pc_src := $(wildcard $(HARDWARE_DIR)/*.c) $(COMMON_DIR)/funcs.c
virt_pc_obj := $(virt_pc_src:.c=.o)
virt_pc_target = pototo

cmp_src = $(foreach dir, $(BYTEARRAY_DIR) $(TRANSLATOR_DIR) $(COMPILER_DIR) $(STR_FUNCS_DIR) $(COMMON_DIR), $(wildcard $(dir)/*.c))
cmp_src += $(LEXER_SRC)
cmp_obj := $(cmp_src:.c=.o)
compiler_target = pan

TEST_SRC := $(foreach dir, $(TESTS_DIR) $(TESTS_LEXER_DIR), $(wildcard $(dir)/*.c))
TEST_OBJ := $(TEST_SRC:.c=.o)
TEST_TARGET = tests_for_all

TARGETS = $(virt_pc_target) $(compiler_target)


all: $(TARGETS)
test: $(TEST_TARGET)

$(virt_pc_target): $(virt_pc_obj)
> $(CC) $(CFLAGS) $^ -o $@

$(compiler_target): $(cmp_obj)
> $(CC) $(CFLAGS) $^ -o $@

gcov_report:
> @mkdir -p $(GCOV_DIR)
#> $(LOCAL_GCOVR) -f s21_.*\.c -f .*/s21_.*\.c --html-details -o $(GCOV_DIR)/$(REPORT)
> $(LOCAL_GCOVR) -e $(TESTS_DIR)\/ --html-details -o $(GCOV_DIR)/$(REPORT)
> ln -s $(GCOV_DIR)/$(REPORT) ./$(REPORT) || true
> @printf "\n\tREPORT FILE FOR OPEN: '\033[38;5;46m$(REPORT)\033[0m'\n"
#> @printf "\trun '\033[38;5;46mmake open_report\033[0m' for open'\n"
> @echo;


$(TEST_TARGET): $(TEST_SRC) $(LEXER_SRC)
> $(CC) -g $^ $(TEST_FLAGS) $(GCOV_FLAGS) -o $@


clean:
> $(RM) $(virt_pc_obj)
> $(RM) $(cmp_obj)
> $(RM) $(TEST_OBJ)
> $(RM) $(wildcard *.gcda)
> $(RM) $(wildcard *.gcno)

clean_all: clean
> $(RM) $(TARGETS)
> $(RM) $(TEST_TARGET)
> $(RM) $(wildcard *.html)
> $(RM) $(wildcard ./$(GCOV_DIR)/*)
> rmdir $(GCOV_DIR)

show:
> @echo "vPC data:"
> @echo $(virt_pc_src)
> @echo $(virt_pc_obj)
> @echo ""
> @echo "Compiler data:"
> @echo $(cmp_src)
> @echo $(cmp_obj)
> @echo ""
> @echo "Tests data:"
> @echo $(TEST_SRC)
> @echo $(TEST_OBJ)

