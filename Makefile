.RECIPEPREFIX = >
.PHONY = all

cmp=gcc
flags=-Wall -Werror -Wextra -std=c1x

src_dir=src
cmp_dir=$(src_dir)/compiler
mem_manager_dir=$(src_dir)/mem_manager
str_funcs_dir=$(src_dir)/str_funcs

obj_dir=objects
build_dir=build


all: virt_pc cmp

virt_pc: $(src_dir)/pc.c $(src_dir)/instructions.c
> $(cmp) $(flags) -g $? -o $(build_dir)/$@

cmp: $(cmp_dir)/*.c $(mem_manager_dir)/*.c $(str_funcs_dir)/*.c
> $(cmp) $(flags) -g $? -o $(build_dir)/$@


clean:
> rm -f $(obj_dir)/*.o

clean_all: clean
> rm $(build_dir)/*
