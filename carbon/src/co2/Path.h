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
#ifndef Path_H
#define Path_H

#include "co2/CompileObject.h"

O_METHOD_DEF (Path, struct String *, to_system_path, (void *_self));

#define PathClass_Attr				\
  CompileObjectClass_Attr;			\
  O_METHOD(Path, to_system_path)

#define Path_Attr				\
  CompileObject_Attr;				\
  struct RefList * path_name

O_CLASS (Path, CompileObject);

#endif /* Path_H */
