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
#ifndef ImplA_H
#define ImplA_H

#include "co2/Object.h"
#include "co2/IfA.h"

#define ImplAClass_Attr				\
  ObjectClass_Attr;				\
  O_METHOD (IfA, getValue);			\
  O_METHOD (IfA, setValue)

#define ImplA_Attr				\
  Object_Attr;					\
  int value

O_CLASS(ImplA, Object);

#endif /* ImplA_H */
