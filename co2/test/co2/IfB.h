/*
 * libco2 Library for object oriented programming in C
 *
 * Copyright (C) 2011,2012 Peterpaul Taekele Klein Haneveld
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
#ifndef IfB_H
#define IfB_H

#include "co2/Interface.h"

//O_METHOD_DEF(IfB, void *, getNext, (void *_self));
//O_METHOD_DEF(IfB, bool, hasNext, (void *_self));

#define IfBClass_Attr				\
  InterfaceClass_Attr

#define IfB_Attr					\
  Interface_Attr
//  O_METHOD(IfB, getNext);			\
//  O_METHOD(IfB, hasNext)

O_CLASS(IfB, Interface);

#endif /* IfB_H */
