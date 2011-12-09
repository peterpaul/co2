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
#include "co2/MyInterfaceImpl.h"

#define O_SUPER Object()

O_IMPLEMENT(MyInterfaceImpl, void *, ctor, (void *_self, va_list * argp))
{
	struct MyInterfaceImpl *self = o_cast(_self, MyInterfaceImpl());
	self = O_SUPER->ctor(self, argp);
	self->value = va_arg(*argp, int);
	return self;
}

O_IMPLEMENT_IF(MyInterfaceImpl, int, getValue, (void *_self), (_self))
{
	struct MyInterfaceImpl *self = O_CAST(_self, MyInterfaceImpl());
	return self->value;
}

O_OBJECT(MyInterfaceImpl, Object);
O_OBJECT_METHOD(MyInterfaceImpl, ctor);
O_OBJECT_METHOD(MyInterfaceImpl, getValue);
/* implement MyInterface */
O_OBJECT_IF(MyInterface);
O_OBJECT_IF_METHOD(MyInterfaceImpl, getValue);
O_OBJECT_IF_END
O_END_OBJECT
