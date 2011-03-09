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
#ifndef SINGLETON_H_
#define SINGLETON_H_

#include "co2/Object.h"

O_METHOD_DEF (Singleton, void *, instance, (void *_self));

#define SingletonClass_Attr			\
  ObjectClass_Attr;				\
  O_METHOD (Singleton, instance);		\
  void * singleton

#define Singleton_Attr				\
  Object_Attr

O_CLASS(Singleton, Object);

#endif				/* SINGLETON_H_ */
