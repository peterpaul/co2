#ifndef _IO_H
#define _IO_H

#include "co2/String.h"
#include "co2/RefList.h"
#include "co2/RefList.h"

extern FILE *out;

extern struct String *base_dir;
extern struct String *file_path;

extern struct RefList *path;

FILE *open_output (const char *filename);

/**
 * Analyzes the filename, and tries to get the full path of the filename
 * @param filename 
 * @return full path of the file
 */
struct String *analyze_file_name (const char *filename);

/**
 * Returns the basedir of the supplied path.
 * @param path
 * @return the basedir of the path
 */
struct String *determine_base_dir (struct String *path);

#endif /* _IO_H */
