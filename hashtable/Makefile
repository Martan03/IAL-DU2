CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -fsanitize=address -g
FILES=hashtable.c test.c test_util.c

.PHONY: test clean

test: $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES)
	./test

clean:
	rm -f test
