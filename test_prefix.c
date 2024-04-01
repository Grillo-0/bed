/*
 * Copyright Arthur Grillo (c) 2024
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>

#define BED_FUNC_PREFIX prefix
#include "bed.h"

int main(void) {
	size_t size;
	unsigned char *blob = prefix_bed_get("test_prefix.c", &size);
	printf("%.*s", (int)size, (char*)blob);
}
