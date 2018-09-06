CC=clang
CFLAGS=-O3 -std=gnu99 -Wall -pedantic

ispl: src/hash*
	$(CC) $(CFLAGS) -lm *.c src/*.c

debug: src/hash*
	$(CC) -g -lm *.c src/*.c

src/hash*:
	git clone https://github.com/Armok628/semstable
	cp semstable/src/hash* src/
	rm -rf semstable

clean:
	rm src/hash*
