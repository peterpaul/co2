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
#ifndef RELEASEPOOL_H
#define RELEASEPOOL_H

#include "co2/Object.h"

O_METHOD_DEF(ReleasePool, void *, add, (void *_self, void *_item));
O_METHOD_DEF(ReleasePool, void *, remove, (void *_self, void *_item));

#define ReleasePoolClass_Attr						\
  ObjectClass_Attr;							\
  O_METHOD (ReleasePool, add);						\
  O_METHOD (ReleasePool, remove)

#define ReleasePool_Attr						\
  Object_Attr;								\
  struct ReleasePool *parent;						\
  struct ReleasePoolItem **hashmap

O_CLASS(ReleasePool, Object);

extern struct ReleasePool * current_release_pool;

#endif				/* not RELEASEPOOL_H */
