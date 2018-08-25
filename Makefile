CC=clang
CFLAGS=-s -O3 -pedantic

ispl: *.c src/*
	$(CC) $(CFLAGS) -lm *.c src/*.c
