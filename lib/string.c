/*
 * Copyright (C) 2013 Bartosz Golaszewski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <busybus.h>
#include "error.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_BUFSIZE		128
#define MAX_ITERATIONS		8

char* bbus_build_string(const char* fmt, ...)
{
	char* buf;
	va_list va;
	int r;
	ssize_t newbufsize;
	int i;
	char* newbuf;

	buf = bbus_malloc(INITIAL_BUFSIZE);
	if (buf == NULL)
		return NULL;
	memset(buf, 0, INITIAL_BUFSIZE);
	va_start(va, fmt);
	r = vsnprintf(buf, INITIAL_BUFSIZE, fmt, va);
	va_end(va);

	if (r >= INITIAL_BUFSIZE) {
		newbufsize = INITIAL_BUFSIZE;
		for (i = 0; i < MAX_ITERATIONS; ++i) {
			newbufsize *= 2;
			newbuf = bbus_realloc(buf, newbufsize);
			if (newbuf == NULL)
				goto errout_free;
			buf = newbuf;
			va_start(va, fmt);
			r = vsnprintf(buf, newbufsize, fmt, va);
			va_end(va);
			if (r < newbufsize)
				break;
		}
		if (r >= newbufsize) {
			__bbus_set_err(BBUS_NOSPACE);
			goto errout_free;
		}
	}

	return buf;

errout_free:
	bbus_free(buf);
	return NULL;
}

char* bbus_copy_string(const char* str)
{
	char* s;
	size_t size;

	size = strlen(str)+1;
	s = bbus_malloc(size);
	if (s == NULL)
		return NULL;
	memset(s, 0, size);
	strncpy(s, str, size);

	return s;
}

void bbus_free_string(char* str)
{
	bbus_free(str);
}

