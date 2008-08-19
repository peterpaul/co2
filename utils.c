#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include "utils.h"

void __assert_fail(const char * assertion, 
		   const char * file, 
		   unsigned int line, 
		   const char * function)
{
	fprintf(stderr, "%s:%d: %s: Assertion `%s' failed.\n",
		file, line, function, assertion);
	fflush(stdout);
	fflush(stderr);
	abort();
}

void __write_message(const char * fmt,
		     const char * file,
		     unsigned int line,
		     const char * function,
		     ...)
{
	va_list ap;
	fprintf(stderr, "%s:%d: %s: ", file, line, function);
	va_start (ap, function);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	va_end(ap);
}

/* djb2 hashing function */
unsigned long hash_function (const unsigned char *str)
{
        unsigned long hash = 5381;
        int c;

        while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash;
}
