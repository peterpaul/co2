#ifndef _IO_H
#define _IO_H

#include "Object.h"
#include "lex.yy.h"

extern FILE * in;
extern FILE * out;

extern const char * filename;

FILE * open_input (const char * filename);
FILE * open_output (const char * filename);

#endif /* _IO_H */
