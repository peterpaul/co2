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
#include "MyObject.h"

#define O_SUPER Object()

O_IMPLEMENT(MyObject, void *, ctor, (void *_self, va_list * argp)) {
	struct MyObject *self = O_CAST(_self, MyObject());
	self->value = va_arg(*argp, int);
	return self;
}

O_IMPLEMENT(MyObject, int, getValue, (void *_self)) {
	struct MyObject *self = O_CAST(_self, MyObject());
	return self->value;
}

O_IMPLEMENT(MyObject, int, setValue, (void *_self, int value)) {
	struct MyObject *self = O_CAST(_self, MyObject());
	self->value = value;
	return value;
}

O_OBJECT(MyObject, Object);
O_OBJECT_METHOD(MyObject, ctor);
O_OBJECT_METHOD(MyObject, getValue);
O_OBJECT_METHOD(MyObject, setValue);
O_OBJECT_END
