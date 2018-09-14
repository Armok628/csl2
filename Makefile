CC=clang
CFLAGS=-O3 -std=gnu99 -Wall -Wextra -Wpedantic

a.out: src/hash*
	$(CC) $(CFLAGS) -lm *.c src/*.c

debug: src/hash*
	$(CC) -g -lm *.c src/*.c

src/hash*:
	curl https://raw.githubusercontent.com/Armok628/semstable/master/src/hash.c > src/hash.c
	curl https://raw.githubusercontent.com/Armok628/semstable/master/src/hash.h > src/hash.h

clean:
	rm -f a.* src/hash*

update:
	make clean
	git pull
	make

install: a.out
	sudo ln -sf `pwd`/a.out /usr/local/bin/ispl

uninstall:
	sudo rm -f /usr/local/bin/ispl
