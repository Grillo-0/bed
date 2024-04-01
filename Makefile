CC=gcc
CFLAGS=-Wall -Wextra -pedantic -ggdb

bed: bed.c
	$(CC) $(CFLAGS) -o bed bed.c

test: test_basic test_prefix

test_basic: bed test.c
	./bed bed.c bed.h test.c bed > resource.c
	$(CC) $(CFLAGS) test.c resource.c -o test_basic
	./test_basic

test_prefix: bed test_prefix.c
	./bed -p prefix bed.c bed.h test_prefix.c bed > resource_prefix.c
	$(CC) $(CFLAGS) test_prefix.c resource_prefix.c -o test_prefix
	./test_prefix

.PHONY: test test_prefix test_basic
