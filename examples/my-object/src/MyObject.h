/*
 * my-object Example project for libco2
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
#ifndef _MYOBJECT_H
#define _MYOBJECT_H

#include "co2/Object.h"

O_METHOD_DEF(MyObject, int, getValue, (void *_self));
O_METHOD_DEF(MyObject, int, setValue, (void *_self, int value));

#define MyObjectClass_Attr			\
	ObjectClass_Attr;			\
	O_METHOD(MyObject, getValue);		\
	O_METHOD(MyObject, setValue)

#define MyObject_Attr				\
	Object_Attr;				\
	int value

O_CLASS(MyObject, Object);

#endif	/* _MYOBJECT_H */
