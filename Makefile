CC=clang
CFLAGS=-g -pedantic

test: *.c src/*
	$(CC) $(CFLAGS) -lm *.c src/*.c
