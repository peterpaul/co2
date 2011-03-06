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
#ifndef Hash_H
#define Hash_H

#include "co2/Object.h"

/* The Tuple is used to store key value pairs
 * and manage a list of Tuples.
 */
#define TupleClass_Attr							\
	ObjectClass_Attr

#define Tuple_Attr								\
	Object_Attr;								\
	char * key;									\
	struct Object * value;						\
	struct Tuple * next

O_CLASS(Tuple, Object);

/* The Hash */
#ifndef HASH_SIZE
#define HASH_SIZE 32
#endif

O_METHOD_DEF(Hash, void *, add, (void *_self, const char *key, const void *_item));
O_METHOD_DEF(Hash, void *, get, (void *_self, const char *key));
O_METHOD_DEF(Hash, void *, set, (void *_self, const char *key, const void *_item));
O_METHOD_DEF(Hash, void *, del, (void *_self, const char *key));
O_METHOD_DEF(Hash, void, error_already_declared, (void *_self, const char *key, const void *_item));

#define HashClass_Attr							\
	ObjectClass_Attr;							\
	O_METHOD (Hash, add);						\
	O_METHOD (Hash, get);						\
	O_METHOD (Hash, set);						\
	O_METHOD (Hash, del);						\
	O_METHOD (Hash, error_already_declared)

#define Hash_Attr								\
	Object_Attr;								\
	struct Tuple *map[HASH_SIZE]

O_CLASS(Hash, Object);

#endif				/* Hash_H */
