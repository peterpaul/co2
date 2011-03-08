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
#ifndef Set_H
#define Set_H

#include "co2/RefObject.h"

O_METHOD_DEF (Set, void *, resize, (void *_self, unsigned size));
O_METHOD_DEF (Set, void *, append, (void *_self, void *item));
O_METHOD_DEF (Set, void *, append_list, (void *_self, void *_list));
O_METHOD_DEF (Set, void *, merge, (void *_self, void *_other));
O_METHOD_DEF (Set, void *, remove, (void *self));
O_METHOD_DEF (Set, void *, get, (void *_self, unsigned index));
O_METHOD_DEF (Set, void *, set, (void *_self, unsigned index, void *item));
O_METHOD_DEF (Set, void *, map, (void *_self, void (*fun) (void *)));
O_METHOD_DEF (Set, void *, map_args,
	      (void *_self, void (*fun) (void *, va_list *), ...));
O_METHOD_DEF (Set, void *, filter, (void *_self, int (*filter) (void *)));
O_METHOD_DEF (Set, void *, filter_args,
	      (void *_self, int (*filter) (void *, va_list *), ...));
O_METHOD_DEF (Set, void *, destroy, (void *_self));
O_METHOD_DEF (Set, void *, getIterator, (void *_self));

#define SetClass_Attr							\
  RefObjectClass_Attr;							\
  O_METHOD (Set, resize);						\
  O_METHOD (Set, append);						\
  O_METHOD (Set, append_list);					\
  O_METHOD (Set, merge);						\
  O_METHOD (Set, remove);						\
  O_METHOD (Set, get);						\
  O_METHOD (Set, set);						\
  O_METHOD (Set, map);						\
  O_METHOD (Set, map_args);						\
  O_METHOD (Set, filter);						\
  O_METHOD (Set, filter_args);					\
  O_METHOD (Set, destroy);						\
  O_METHOD (Set, getIterator)

#define Set_Attr				\
  RefObject_Attr;				\
  struct Class * type;				\
  struct RefObject ** data;			\
  unsigned length, max

O_CLASS (Set, RefObject);

#endif /* Set_H */
