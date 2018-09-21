CC=clang
CFLAGS=-O3 -std=gnu99 -Wall -Wextra -Wpedantic
LIBFILE=`pwd`/functions.lisp

a.*: src/hash* src/libfilename.h src/* main.c 
	$(CC) $(CFLAGS) -lm *.c src/*.c

debug: src/hash* src/libfilename.h src/* main.c
	$(CC) -g -lm *.c src/*.c

src/libfilename.h:
ifdef $(LIBFILE)
	echo "#define LIBRARY_FILE \"$(LIBFILE)\"" > src/libfilename.h
else
	echo "" > src/libfilename.h
endif

src/hash*:
	curl https://raw.githubusercontent.com/Armok628/semstable/master/src/hash.c > src/hash.c
	curl https://raw.githubusercontent.com/Armok628/semstable/master/src/hash.h > src/hash.h

clean:
	rm -f a.* src/hash* src/libfilename.h

update:
	make clean
	git pull
	make

install: a.*
	sudo ln -sf `pwd`/a.* /usr/local/bin/ispl

uninstall:
	sudo rm -f /usr/local/bin/ispl
