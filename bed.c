/*
 * Copyright Arthur Grillo (c) 2024
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "bed.h"

#define COLUMNS 12

#define DA_EXPAND_FACTOR 2

#define DA_DEFINE(item_type) struct { \
	item_type *items; \
	size_t len; \
	size_t capacity; \
}

#define da_append(da, item) do { \
	if ((da)->len == (da)->capacity) { \
		if ((da)->capacity == 0) \
			(da)->capacity = 1; \
		(da)->capacity *= DA_EXPAND_FACTOR; \
		(da)->items = realloc((da)->items, sizeof(*(da)->items) * (da)->capacity); \
	} \
	(da)->items[(da)->len] = (item); \
	(da)->len++; \
} while (0)

static void print_help(char *prg_name) {
	printf("usage: %s <file>...\n", prg_name);
	printf("Embed files into a C program by creating a .c file\n");
}

static size_t get_file_size(char *file_name) {
	int ret;
	FILE *fp = fopen(file_name, "r");
	assert(fp != NULL);

	ret = fseek(fp, 0, SEEK_END);
	assert(ret != -1);

	int size = ftell(fp);
	assert(size != -1);

	ret = fclose(fp);
	assert(ret != EOF);

	return size;
}

#define write_to_c(...) __write_to_c(__FILE__, __LINE__, __VA_ARGS__)

static void __write_to_c(char *file_path, int line_num, const char *fmt, const char *end, ...) {
	va_list ap;
	va_start(ap, end);
	vprintf(fmt, ap);
	va_end(ap);
	printf("\t// %s:%d%s", file_path, line_num, end);
}

static DA_DEFINE(struct bed_file) metadata;
static unsigned char *storage;

int main(int argc, char *argv[]) {
	if (argc == 1) {
		fprintf(stderr, "%s: missing files\n", argv[0]);
		print_help(argv[0]);
		exit(EXIT_FAILURE);
	}

	// char* prg_name = *argv;
	argv++;
	argc--;

	size_t total_size = 0;
	for (int i = 0; i < argc; i++) {
		struct bed_file f = {
			.name = argv[i],
			.size = get_file_size(argv[i]),
			.offset = i == 0 ? 0 : total_size,
		};

		size_t pg_size = getpagesize();
		total_size += (f.size + pg_size) / pg_size * pg_size;

		da_append(&metadata, f);
	}

	storage = aligned_alloc(getpagesize(), total_size);
	assert(storage != NULL);

	for (size_t i = 0; i < metadata.len; i++) {
		struct bed_file *f = &metadata.items[i];

		if (f->size == 0)
			continue;

		int fd = open(f->name, O_RDONLY);
		assert(fd != -1);

		void* pa;
		pa = mmap(storage + f->offset, f->size, PROT_READ, MAP_PRIVATE | MAP_FIXED, fd, 0);
		if (pa == MAP_FAILED) {
			perror("mmap");
			assert(0);
		}

		int ret = close(fd);
		assert(ret != -1);
	}

	write_to_c("#include \"bed.h\"", "\n");
	write_to_c("struct bed_file metadata[] = {", "\n\t");
	for (size_t i = 0; i < metadata.len; i++) {
		struct bed_file *f = &metadata.items[i];
		write_to_c("{.name = \"%s\", .size = %zu, .offset = %zu},", "\n", f->name, f->size, f->offset);
		if(i < metadata.len - 1) printf("\t");
	}
	write_to_c("};", "\n");

	write_to_c("unsigned char resource[] = {", "\n\t");
	for (size_t i = 0; i < total_size; i++) {
		printf("0x%02x,", storage[i]);
		if ((i + 1) % COLUMNS == 0) {
			write_to_c("","\n");
			if(i < total_size - 1) printf("\t");
		}
		else {
			printf(" ");
		}
	}

	write_to_c("};", "\n");
	write_to_c("#define NUM_FILES %d", "\n", metadata.len);
	write_to_c("#define __INTERNAL_DONT_DEFINE_THIS", "\n");
	write_to_c("#include \"bed.h\"", "\n");

	exit(EXIT_SUCCESS);
}
