CC=gcc
CFLAGS=-Wall -Wextra -pedantic -ggdb

bed: bed.c
	$(CC) $(CFLAGS) -o bed bed.c

test: bed test.c
	./bed bed.c bed.h test.c bed > resource.c
	$(CC) $(CFLAGS) test.c resource.c -o test
	./test

.PHONY: test
