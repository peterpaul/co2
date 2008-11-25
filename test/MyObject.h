/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _MYOBJECT_H
#define _MYOBJECT_H

#include "Object.h"

O_METHOD_DEF(MyObject, int, getValue, (void *_self));
O_METHOD_DEF(MyObject, int, setValue, (void *_self, int value));

#define MyObjectClass_Attr					\
	ObjectClass_Attr;						\
	O_METHOD(MyObject, getValue);			\
	O_METHOD(MyObject, setValue)

#define MyObject_Attr						\
	Object_Attr;							\
	int value

O_CLASS(MyObject, Object);

#endif				/* _MYOBJECT_H */
