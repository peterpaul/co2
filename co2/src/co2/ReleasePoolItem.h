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
#ifndef RELEASEPOOLITEM_H
#define RELEASEPOOLITEM_H

#include "co2/Object.h"

O_METHOD_DEF(ReleasePoolItem, void *, add, (void *_self, void *_item));
O_METHOD_DEF(ReleasePoolItem, void *, remove, (void *_self, void *_item));
O_METHOD_DEF(ReleasePoolItem, void, clear_list, (void *_self));

#define ReleasePoolItemClass_Attr					\
  ObjectClass_Attr;							\
  O_METHOD (ReleasePoolItem, add);					\
  O_METHOD (ReleasePoolItem, remove);					\
  O_METHOD (ReleasePoolItem, clear_list)

#define ReleasePoolItem_Attr						\
  Object_Attr;								\
  struct ReleasePoolItem *next;						\
  struct RefObject *item

O_CLASS(ReleasePoolItem, Object);

#endif				/* not RELEASEPOOLITEM_H */
