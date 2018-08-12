CC=clang
CFLAGS=-g -pedantic
CORES=src/core.h src/arith.h

test: *.c src/*
	tclsh gen_init.tcl $(CORES) > init.h
	$(CC) $(CFLAGS) *.c src/*.c
