CC=clang
CFLAGS=-s -Oz -pedantic

ispl: *.c src/*
	$(CC) $(CFLAGS) -lm *.c src/*.c
