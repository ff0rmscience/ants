tm : tm.c
	clang-12 tm.c -o tm -g -I/usr/include/x86_64-linux-gnu \
        -lallegro -lallegro_primitives
	./tm
