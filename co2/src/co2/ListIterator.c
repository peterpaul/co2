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
#include "co2/ListIterator.h"

#define O_SUPER Object()

O_IMPLEMENT(ListIterator, void *, ctor, (void *_self, va_list * argp))
{
	struct ListIterator *self = O_CAST(_self, ListIterator());
	self = O_SUPER->ctor(self, argp);
	self->list = o_cast(va_arg(*argp, void *), List());
	self->pos = 0;
	return self;
}

O_IMPLEMENT_IF(ListIterator, void *, getNext, (void *_self), (_self))
{
	struct ListIterator *self = O_CAST(_self, ListIterator());
	void *result = NULL;
	while (self->pos < self->list->length
	       && (result = self->list->data[self->pos]) == NULL) {
		self->pos++;
	}
	if (self->pos < self->list->length) {
		self->pos++;
		return result;
	}
	return NULL;
}

O_IMPLEMENT_IF(ListIterator, bool, hasNext, (void *_self), (_self))
{
	struct ListIterator *self = O_CAST(_self, ListIterator());
	while (self->pos < self->list->length
	       && self->list->data[self->pos] == NULL) {
		self->pos++;
	}
	return self->pos < self->list->length;
}

O_OBJECT(ListIterator, Object);
O_OBJECT_METHOD(ListIterator, ctor);
O_OBJECT_METHOD(ListIterator, getNext);
O_OBJECT_METHOD(ListIterator, hasNext);
O_OBJECT_IF(Iterator);
O_OBJECT_IF_METHOD(ListIterator, getNext);
O_OBJECT_IF_METHOD(ListIterator, hasNext);
O_OBJECT_IF_END
O_END_OBJECT
