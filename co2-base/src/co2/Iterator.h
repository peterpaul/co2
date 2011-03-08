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
#ifndef ITERATOR_H
#define ITERATOR_H

#include "co2/Interface.h"

O_METHOD_DEF(Iterator, void *, getNext, (void *_self));
O_METHOD_DEF(Iterator, bool, hasNext, (void *_self));

#define IteratorClass_Attr			\
	InterfaceClass_Attr

#define Iterator_Attr				\
	Interface_Attr;				\
	O_METHOD(Iterator, getNext);		\
	O_METHOD(Iterator, hasNext)

O_CLASS(Iterator, Interface);

#endif /* ITERATOR_H */
