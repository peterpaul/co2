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
#ifndef Type_H
#define Type_H

#include "co2/CompileObject.h"

O_METHOD_DEF (Type, bool, is_compatible, (void *_self, void *_other));
O_METHOD_DEF (Type, void, assert_compatible, (void *_self, void *_other));
O_METHOD_DEF (Type, void, assume_compatible, (void *_self, void *_other));
O_METHOD_DEF (Type, bool, is_void_ptr, (void *_self));
O_METHOD_DEF (Type, struct Type *, get_declared_type, (void *_self));

#define TypeClass_Attr				\
  CompileObjectClass_Attr;			\
  O_METHOD(Type, is_compatible);		\
  O_METHOD(Type, is_void_ptr);			\
  O_METHOD(Type, assert_compatible);		\
  O_METHOD(Type, assume_compatible);		\
  O_METHOD(Type, get_declared_type)

#define Type_Attr				\
  CompileObject_Attr

O_CLASS (Type, CompileObject);

#endif /* Type_H */
