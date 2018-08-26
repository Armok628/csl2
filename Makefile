CC=clang
CFLAGS=-s -O3 -pedantic

ispl: *.c src/*
	$(CC) $(CFLAGS) -lm *.c src/*.c

debug: *.c src/*
	$(CC) -g -lm *.c src/*.c
