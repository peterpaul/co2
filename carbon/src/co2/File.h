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
#ifndef File_H
#define File_H

#include "co2/CompileObject.h"
#include "co2/Hash.h"

O_METHOD_DEF (File, void, sort, (void *_self));

#define FileClass_Attr				\
  CompileObjectClass_Attr;			\
  O_METHOD (File, sort)

#define File_Attr				\
  CompileObject_Attr;				\
  struct String * name;				\
  struct String * absolute_path;		\
  struct RefList * file_dependencies;		\
  struct RefList * declarations

O_CLASS (File, CompileObject);

struct Hash * get_hash_map ();

#endif /* File_H */
