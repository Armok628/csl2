CC=clang
CFLAGS=-Oz -pedantic

test: *.c src/*
	$(CC) $(CFLAGS) -lm *.c src/*.c
