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
#include "co2/MySingleton.h"

#define O_SUPER Singleton()

void *MySingleton_ctor(void *_self, va_list * argp)
{
	struct MySingleton *self = o_cast(_self, MySingleton());
	self = O_SUPER->ctor(self, argp);
	self->value = va_arg(*argp, int);
	return self;
}

O_OBJECT(MySingleton, Singleton);
self->ctor = MySingleton_ctor;
O_OBJECT_END
