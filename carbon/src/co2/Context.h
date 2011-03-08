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
#ifndef Context_H
#define Context_H

#include "co2/RefList.h"

O_METHOD_DEF (Context, void, add, (void *_self, void *_item));
O_METHOD_DEF (Context, void *, find, (void *_self, void *_class));
O_METHOD_DEF (Context, void, remove_last, (void *_self));

#define ContextClass_Attr			\
  RefObjectClass_Attr;				\
  O_METHOD(Context, add);			\
  O_METHOD(Context, find);			\
  O_METHOD(Context, remove_last)

#define Context_Attr				\
  RefObject_Attr;				\
  struct RefList * stack

O_CLASS (Context, RefObject);

extern struct Context *current_context;

#endif /* Context_H */
