#ifndef _IO_H
#define _IO_H

#include "String.h"
#include "lex.yy.h"

extern FILE * out;

extern const char * filename;

extern struct String * base_dir;
extern struct String * file_path;

FILE * open_input (const char * filename);
FILE * open_output (const char * filename);

struct String * analyze_file_name(const char * filename);
struct String * determine_base_dir(struct String * path);

#endif /* _IO_H */
