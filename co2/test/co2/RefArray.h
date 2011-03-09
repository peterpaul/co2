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
#ifndef RefArray_H
#define RefArray_H

#include "co2/RefObject.h"

O_METHOD_DEF(RefArray, void *, resize, (void *_self, unsigned capacity));
O_METHOD_DEF(RefArray, void *, set, (void *_self, unsigned index, void *_item));
O_METHOD_DEF(RefArray, void *, get, (void *_self, unsigned index));

#define RefArrayClass_Attr			\
  RefObjectClass_Attr;				\
  O_METHOD(RefArray, resize);			\
  O_METHOD(RefArray, set);			\
  O_METHOD(RefArray, get)

#define RefArray_Attr				\
  RefObject_Attr;				\
  unsigned capacity;				\
  struct RefObject ** data;			\
  struct Class * type

O_CLASS(RefArray, RefObject);

#endif /* RefArray_H */
