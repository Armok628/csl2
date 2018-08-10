CC=clang
CFLAGS=-g -pedantic

test: *.c src/*
	$(CC) $(CFLAGS) *.c src/*.c
