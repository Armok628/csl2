CC=clang
CFLAGS=-O3 -std=gnu99 -Wall -Wextra -Wpedantic
LIBFILE=`pwd`/functions.lisp

a.*: src/hash* src/* main.c 
ifeq ($(strip $(LIBFILE)),)
	$(CC) $(CFLAGS) -lm *.c src/*.c
else
	$(CC) $(CFLAGS) "-DLIBFILE=\"$(LIBFILE)\"" -lm *.c src/*.c
endif

src/hash*:
	curl https://raw.githubusercontent.com/Armok628/semstable/master/src/hash.c > src/hash.c
	curl https://raw.githubusercontent.com/Armok628/semstable/master/src/hash.h > src/hash.h

clean:
	rm -f a.* src/hash*

update:
	make clean
	git pull
	make

install: a.*
	ln -sf `pwd`/a.* /usr/local/bin/ispl

uninstall:
	rm -f /usr/local/bin/ispl
