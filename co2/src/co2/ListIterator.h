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
#ifndef LISTITERATOR_H
#define LISTITERATOR_H

#include "co2/Iterator.h"
#include "co2/List.h"

#define ListIteratorClass_Attr			\
	ObjectClass_Attr;			\
	O_METHOD(Iterator, getNext);		\
	O_METHOD(Iterator, hasNext)

#define ListIterator_Attr			\
	Object_Attr;				\
	struct List * list;			\
	int pos

O_CLASS(ListIterator, Object);

#endif /* LISTITERATOR_H */
