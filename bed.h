#ifndef __BED_H__

#include <stddef.h>

#ifndef __INTERNAL_DONT_DEFINE_THIS

struct bed_file {
	char *name;
	size_t size;
	size_t offset;
};

#endif // __INTERNAL_DONT_DEFINE_THIS

void bed_get(char *file_name, unsigned char **blob, size_t *size);

#ifdef __INTERNAL_DONT_DEFINE_THIS

#include <string.h>
#include <stdbool.h>

void bed_get(char* file_name, unsigned char** blob, size_t *size) {
	struct bed_file *f;
	bool found = false;
	for (int i = 0; i < NUM_FILES; i++) {
		if(!strcmp(metadata[i].name, file_name)) {
			found = true;
			f = &metadata[i];
			break;
		}
	}

	if (found) {
		*blob = &resource[f->offset];
		*size = f->size;
	} else {
		*blob = NULL;
		*size = 0;
	}
}

#endif // __INTERNAL_DONT_DEFINE_THIS

#endif // __BED_H__
