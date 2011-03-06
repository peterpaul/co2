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
#include "co2/Interface.h"

#define O_SUPER Object()

O_IMPLEMENT(Interface, void *, ctor, (void *_self, va_list * app))
{
	struct Interface *self = O_CAST(_self, Interface());
	self = O_SUPER->ctor(self, app);
	self->name = va_arg(*app, char *);
	self->next = o_branch_cast(va_arg(*app, struct Interface *), Interface());
	return self;
}

O_OBJECT(Interface, Object);
O_OBJECT_METHOD(Interface, ctor);
O_END_OBJECT
