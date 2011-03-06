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
#include "co2/ReleasePool.h"
#include "co2/RefObject.h"
#include "co2/ReleasePoolItem.h"

#define O_SUPER Object()

struct ReleasePool * current_release_pool;
#define RELEASEPOOL_HASH_SIZE 1223

O_IMPLEMENT(ReleasePool, void *, ctor, (void *_self, va_list * app))
{
  struct ReleasePool *self = O_CAST(_self, ReleasePool());
  self = O_SUPER->ctor(self, app);
  self->parent = current_release_pool;
  current_release_pool = self;
  self->hashmap = calloc(sizeof(struct ReleasePoolItem *), RELEASEPOOL_HASH_SIZE);
  return self;
}

O_IMPLEMENT(ReleasePool, void *, dtor, (void *_self))
{
  struct ReleasePool *self = O_CAST(_self, ReleasePool());
  int i;
  for (i=0; i<RELEASEPOOL_HASH_SIZE; i++)
    {
      struct ReleasePoolItem * list = self->hashmap[i];
      self->hashmap[i] = NULL;
      O_BRANCH_CALL(list, clear_list);
    }
  free(self->hashmap);
  current_release_pool = self->parent;

  self->hashmap = NULL;
  self->parent = NULL;
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(ReleasePool, void *, add, (void *_self, void *_item))
{
  struct ReleasePool *self = O_CAST(_self, ReleasePool());
  size_t index = ((size_t)_item) % RELEASEPOOL_HASH_SIZE;
  struct ReleasePoolItem *item = O_CALL_CLASS(ReleasePoolItem(), new, _item);
  if (self->hashmap[index] == NULL)
    self->hashmap[index] = item;
  else
    self->hashmap[index] = O_CALL(self->hashmap[index], add, item);
  return _item;
}

O_IMPLEMENT(ReleasePool, void *, remove, (void *_self, void *_item))
{
  struct ReleasePool *self = O_CAST(_self, ReleasePool());
  size_t index = ((size_t)_item) % RELEASEPOOL_HASH_SIZE;
  self->hashmap[index] = O_BRANCH_CALL(self->hashmap[index], remove, _item);
  return _item;
}

O_OBJECT(ReleasePool, Object);
O_OBJECT_METHOD(ReleasePool, ctor);
O_OBJECT_METHOD(ReleasePool, dtor);
O_OBJECT_METHOD(ReleasePool, add);
O_OBJECT_METHOD(ReleasePool, remove);
O_END_OBJECT
