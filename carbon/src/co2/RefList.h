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
#ifndef REFLIST_H
#define REFLIST_H

#include "co2/RefObject.h"

O_METHOD_DEF (RefList, void *, resize, (void *_self, unsigned size));
O_METHOD_DEF (RefList, void *, append, (void *_self, void *item));
O_METHOD_DEF (RefList, void *, append_list, (void *_self, void *_list));
O_METHOD_DEF (RefList, void *, merge, (void *_self, void *_other));
O_METHOD_DEF (RefList, void *, remove, (void *self));
O_METHOD_DEF (RefList, void *, get, (void *_self, unsigned index));
O_METHOD_DEF (RefList, void *, set,
	      (void *_self, unsigned index, void *item));
O_METHOD_DEF (RefList, void *, map, (void *_self, void (*fun) (void *)));
O_METHOD_DEF (RefList, void *, map_args,
	      (void *_self, void (*fun) (void *, va_list *), ...));
O_METHOD_DEF (RefList, void *, filter, (void *_self, int (*filter) (void *)));
O_METHOD_DEF (RefList, void *, filter_args,
	      (void *_self, int (*filter) (void *, va_list *), ...));
O_METHOD_DEF (RefList, void *, destroy, (void *_self));
O_METHOD_DEF (RefList, void *, getIterator, (void *_self));

#define RefListClass_Attr						\
  RefObjectClass_Attr;							\
  O_METHOD (RefList, resize);						\
  O_METHOD (RefList, append);						\
  O_METHOD (RefList, append_list);					\
  O_METHOD (RefList, merge);						\
  O_METHOD (RefList, remove);						\
  O_METHOD (RefList, get);						\
  O_METHOD (RefList, set);						\
  O_METHOD (RefList, map);						\
  O_METHOD (RefList, map_args);						\
  O_METHOD (RefList, filter);						\
  O_METHOD (RefList, filter_args);					\
  O_METHOD (RefList, destroy);						\
  O_METHOD (RefList, getIterator)

#define RefList_Attr							\
  RefObject_Attr;							\
  struct Class * type;							\
  struct RefObject **data;						\
  unsigned length, max

O_CLASS (RefList, RefObject);

#endif /* not REFLIST_H */
