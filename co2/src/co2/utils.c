/*
 * libco2 Library for object oriented programming in C
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include "co2/utils.h"

void __assert_fail(const char *assertion, const char *file, unsigned int line,
		   const char *function)
{
	fprintf(stderr, "%s:%d: %s: Assertion `%s' failed.\n", file, line,
		function, assertion);
	fflush(stdout);
	fflush(stderr);
	abort();
}

void __write_message(const char *fmt, const char *file, unsigned int line,
		     const char *function, ...)
{
	va_list ap;
	fprintf(stderr, "%s:%d: %s: ", file, line, function);
	va_start(ap, function);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

/* djb2 hashing function */
unsigned long hash_function(const unsigned char *str)
{
	unsigned long hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;	/* hash * 33 + c */

	return hash;
}

int ipow(int base, int exp)
{
  int result = 1;
  while (exp)
    {
      if (exp & 1)
	result *= base;
      exp >>= 1;
      base *= base;
    }

  return result;
}

unsigned upow(unsigned base, int exp)
{
  unsigned result = 1;
  while (exp)
    {
      if (exp & 1)
	result *= base;
      exp >>= 1;
      base *= base;
    }

  return result;
}
