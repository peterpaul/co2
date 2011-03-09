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
#include "co2/MyObject.h"

#define O_SUPER Object()

void *MyObject_ctor(void *_self, va_list * argp)
{
	struct MyObject *self = o_cast(_self, MyObject());
	self = O_SUPER->ctor(self, argp);
	self->value = va_arg(*argp, int);
	return self;
}

int MyObject_getValue(void *_self)
{
	struct MyObject *self = o_cast(_self, MyObject());
	return self->value;
}

int MyObject_setValue(void *_self, int value)
{
	struct MyObject *self = o_cast(_self, MyObject());
	self->value = value;
	return value;
}

O_OBJECT(MyObject, Object);
self->ctor = MyObject_ctor;
self->getValue = MyObject_getValue;
self->setValue = MyObject_setValue;
O_OBJECT_END
