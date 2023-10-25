CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -lm -fsanitize=address -g
FILES=btree.c ../btree.c stack.c ../test_util.c ../test.c

.PHONY: test clean

test: $(FILES)
	$(CC) $(CFLAGS) -o $@ $(FILES)
	./test

clean:
	rm -f test
