.RECIPEPREFIX = >
.PHONY = all

cmp=gcc
flags=-Wall -Werror -Wextra -std=c1x

src_dir=src
cmp_dir=$(src_dir)/compiler
common_dir=$(src_dir)/common
mem_manager_dir=$(src_dir)/mem_manager
str_funcs_dir=$(src_dir)/str_funcs
translator_dir=$(src_dir)/tree_translator
test_dir=$(src_dir)/tests

obj_dir=objects
build_dir=build


all: virt_pc cmp
tests: str_funcs_tests

virt_pc: $(src_dir)/pc.c $(src_dir)/instructions.c $(common_dir)/funcs.c
> $(cmp) $(flags) -g $? -o $(build_dir)/$@

cmp: $(cmp_dir)/*.c $(mem_manager_dir)/*.c $(str_funcs_dir)/*.c $(common_dir)/*.c 
> $(cmp) $(flags) -g $? -o $(build_dir)/$@


str_funcs_tests: $(test_dir)/str_funcs_test.c
> $(cmp) -g $? -o $(build_dir)/$@


clean:
> rm -f $(obj_dir)/*.o

clean_all: clean
> rm $(build_dir)/*
