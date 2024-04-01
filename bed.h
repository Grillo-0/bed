/*
 * Copyright Arthur Grillo (c) 2024
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef __BED_H__
#define __BED_H__

#include <stddef.h>

#ifdef BED_FUNC_PREFIX
#define __BED_CONCAT(a, b) a ## _ ## b
#define BED_CONCAT(a, b) __BED_CONCAT(a,b)
#define BED_FUNC(func) BED_CONCAT(BED_FUNC_PREFIX, func)
#else
#define BED_FUNC(func) func
#endif // BED_FUNC_PREFIX

unsigned char* BED_FUNC(bed_get)(char *file_name, size_t *size);

#endif // __BED_H__
