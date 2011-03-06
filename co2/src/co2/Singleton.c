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
/*
 * Singleton.c
 *
 *  Created on: Oct 23, 2008
 *      Author: epetkle
 */

#include "co2/Singleton.h"

#define O_SUPER Object()

void *Singleton_new(void *_self, ...)
{
	struct SingletonClass *self = o_cast(_self, SingletonClass());
	if (!self->singleton) {
		struct Singleton *object = calloc(1, self->size);
		self->singleton = object;
		va_list ap;
		object->class = self;
		va_start(ap, _self);
		/* Call ctor method */
		O_CALL(object, ctor, &ap);
		va_end(ap);
	}
	return self->singleton;
}

void *Singleton_delete(void *_self)
{
	struct Singleton *self = o_cast(_self, Singleton());
	self->class->singleton = NULL;
	return O_SUPER->delete(self);
}

O_OBJECT(Singleton, Object);
self->new = Singleton_new;
self->delete = Singleton_delete;
O_OBJECT_END
