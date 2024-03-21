CC=gcc
CFLAGS=-Wall -Wextra -pedantic -ggdb

bed: bed.c
	$(CC) $(CFLAGS) -o bed bed.c

test: bed test.c
	./bed test.c > resource.c
	$(CC) $(CFLAGS) test.c resource.c -o test
	./test
	rm resource.c

.PHONY: test
