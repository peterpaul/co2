#ifndef _IO_H
#define _IO_H

#include "Object.h"
#include "lex.h"

extern FILE * in;
extern FILE * out;

FILE * open_input (const char * filename);
FILE * open_output (const char * filename);

#endif /* _IO_H */
