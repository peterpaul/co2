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
#ifndef ArrayList_H
#define ArrayList_H

#include "co2/Array.h"

O_METHOD_DEF(ArrayList, void *, prepend, (void *_self, void *item));
O_METHOD_DEF(ArrayList, void *, append, (void *_self, void *item));
O_METHOD_DEF(ArrayList, void *, append_list, (void *_self, void *_list));
O_METHOD_DEF(ArrayList, void *, merge, (void *_self, void *_other));
O_METHOD_DEF(ArrayList, void *, map, (void *_self, void (*fun) (void *)));
O_METHOD_DEF(ArrayList, void *, map_args,
	     (void *_self, void (*fun) (void *, va_list *), ...));
O_METHOD_DEF(ArrayList, void *, filter, (void *_self, int (*filter) (void *)));
O_METHOD_DEF(ArrayList, void *, filter_args,
	     (void *_self, int (*filter) (void *, va_list *), ...));
O_METHOD_DEF(ArrayList, void *, getIterator, (void *_self));

#define ArrayListClass_Attr			\
  ArrayClass_Attr;				\
  O_METHOD(ArrayList, prepend);			\
  O_METHOD(ArrayList, append);			\
  O_METHOD(ArrayList, append_list);		\
  O_METHOD(ArrayList, merge);			\
  O_METHOD(ArrayList, map);			\
  O_METHOD(ArrayList, map_args);		\
  O_METHOD(ArrayList, filter);			\
  O_METHOD(ArrayList, filter_args);		\
  O_METHOD(ArrayList, getIterator)


#define ArrayList_Attr				\
  Array_Attr;					\
  unsigned length

O_CLASS(ArrayList, Array);

#endif /* ArrayList_H */
