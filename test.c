/*
 * Copyright Arthur Grillo (c) 2024
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>

#include "bed.h"

int main(void) {
	size_t size;
	unsigned char *blob = bed_get("test.c", &size);
	printf("%.*s", (int)size, (char*)blob);
}
