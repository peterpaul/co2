/*
 * libco2 Library for object oriented programming in C
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIST_H
#define LIST_H

#include "co2/Object.h"

O_METHOD_DEF(List, void *, resize, (void *_self, unsigned size));
O_METHOD_DEF(List, void *, append, (void *_self, void *item));
O_METHOD_DEF(List, void *, append_list, (void *_self, void *_list));
O_METHOD_DEF(List, void *, merge, (void *_self, void *_other));
O_METHOD_DEF(List, void *, remove, (void *self));
O_METHOD_DEF(List, void *, get, (void *_self, unsigned index));
O_METHOD_DEF(List, void *, set, (void *_self, unsigned index, void *item));
O_METHOD_DEF(List, void *, map, (void *_self, void (*fun) (void *)));
O_METHOD_DEF(List, void *, map_args,
	     (void *_self, void (*fun) (void *, va_list *), ...));
O_METHOD_DEF(List, void *, filter, (void *_self, int (*filter) (void *)));
O_METHOD_DEF(List, void *, filter_args,
	     (void *_self, int (*filter) (void *, va_list *), ...));
O_METHOD_DEF(List, void *, destroy, (void *_self));
O_METHOD_DEF(List, void *, getIterator, (void *_self));

#define ListClass_Attr							\
	ObjectClass_Attr;						\
	O_METHOD (List, resize);					\
	O_METHOD (List, append);					\
	O_METHOD (List, append_list);					\
	O_METHOD (List, merge);						\
	O_METHOD (List, remove);					\
	O_METHOD (List, get);						\
	O_METHOD (List, set);						\
	O_METHOD (List, map);						\
	O_METHOD (List, map_args);					\
	O_METHOD (List, filter);					\
	O_METHOD (List, filter_args);					\
	O_METHOD (List, destroy);					\
	O_METHOD (List, getIterator)

#define List_Attr								\
	Object_Attr;								\
	struct Class * type;						\
	void **data;								\
	unsigned length, max

O_CLASS(List, Object);

#endif				/* not LIST_H */
