/*
 * carbon Compiler and programming language for libco2
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
