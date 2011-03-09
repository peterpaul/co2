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
#ifndef _MY_INTERFACE_H
#define _MY_INTERFACE_H

#include "co2/Interface.h"

O_METHOD_DEF(MyInterface, int, getValue, (void *_self));

#define MyInterfaceClass_Attr					\
	InterfaceClass_Attr

#define MyInterface_Attr						\
	Interface_Attr;								\
	O_METHOD(MyInterface, getValue)

O_CLASS(MyInterface, Interface);

#endif				/* _MY_INTERFACE_H */
