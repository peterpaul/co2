/*
 * libco2 Library for object oriented programming in C
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef Array_H
#define Array_H

#include "co2/RefObject.h"

O_METHOD_DEF(Array, void *, resize, (void *_self, unsigned capacity));
O_METHOD_DEF(Array, void *, set, (void *_self, unsigned index, void *_item));
O_METHOD_DEF(Array, void *, get, (void *_self, unsigned index));

#define ArrayClass_Attr				\
  RefObjectClass_Attr;				\
  O_METHOD(Array, resize);			\
  O_METHOD(Array, set);				\
  O_METHOD(Array, get)

#define Array_Attr				\
  RefObject_Attr;				\
  unsigned capacity;				\
  void ** data;					\
  struct Class * type

O_CLASS(Array, RefObject);

#endif /* Array_H */
