.RECIPEPREFIX = >

cmp=gcc
flags=-Wall -Werror -Wextra -std=c1x

target=virt_pc

$(target): pc.c instructions.c
> $(cmp) $(flags) pc.c instructions.c -o $@

clean_all:
> rm $(target)
