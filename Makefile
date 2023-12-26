.RECIPEPREFIX = >
.PHONY = all

cmp := gcc
flags := -Wall -Werror -Wextra -std=c1x

src_dir := src
cmp_dir := $(src_dir)/compiler
common_dir := $(src_dir)/common
mem_manager_dir := $(src_dir)/mem_manager
str_funcs_dir := $(src_dir)/str_funcs
translator_dir := $(src_dir)/tree_translator
test_dir := $(src_dir)/tests

build_dir := build

virt_pc_src := $(src_dir)/pc.c $(src_dir)/instructions.c $(common_dir)/funcs.c
virt_pc_obj := $(virt_pc_src:.c=.o)

#cmp_src := $(foreach dir, $(cmp_dir) $(mem_manager_dir) $(str_funcs_dir) $(common_dir), $(wildcard $(dir)/*.c))
cmp_src := $(foreach dir, $(translator_dir) $(cmp_dir) $(mem_manager_dir) $(str_funcs_dir) $(common_dir), $(wildcard $(dir)/*.c))
cmp_obj := $(cmp_src:.c=.o)

test_src := $(foreach dir, $(test_dir), $(wildcard $(dir)/*.c))
test_obj := $(test_src:.c=.o)

targets = virt_pc cmp #str_funcs_tests


all: $(targets)
tests: str_funcs_tests

virt_pc: $(virt_pc_obj)
> $(cmp) $(flags) -g $(virt_pc_obj) -o $(build_dir)/$@

cmp: $(cmp_obj)
> $(cmp) $(flags) -g $(cmp_obj) -o $(build_dir)/$@


str_funcs_tests: $(test_obj)
> $(cmp) -g $(test_obj) -o $(build_dir)/$@


clean:
> rm -f $(virt_pc_obj)
> rm -f $(cmp_obj)
> rm -f $(test_obj)

clean_all: clean
> rm -f $(foreach targ, $(targets), $(build_dir)/$(targ))
> rm -f $(foreach test, $(tests), $(build_dir)/$(test))


%.o: %.c
> $(cmp) -c $(flags) $? -o $@

show:
> @echo $(virt_pc_src)
> @echo $(virt_pc_obj)
> @echo ""
> @echo $(cmp_src)
> @echo $(cmp_obj)
> @echo ""
> @echo $(test_src)
> @echo $(test_obj)

