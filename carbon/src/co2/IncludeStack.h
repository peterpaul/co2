1;2790;0c/*
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
#ifndef INCLUDESTACK_H
#define INCLUDESTACK_H

#include "co2/RefObject.h"
#include "co2/File.h"

#define IncludeStackClass_Attr			\
  RefObjectClass_Attr

#define IncludeStack_Attr			\
  RefObject_Attr;				\
  struct IncludeStack * parent;			\
  unsigned current_line;			\
  struct File * current_file;			\
  YY_BUFFER_STATE buffer_state

O_CLASS (IncludeStack, RefObject);

extern int linenumber;

extern struct IncludeStack * include_stack;

#endif /* INCLUDESTACK_H */
