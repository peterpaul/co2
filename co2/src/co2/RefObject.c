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
#include "co2/RefObject.h"

#define O_SUPER Object()

O_IMPLEMENT(RefObject, void *, ctor, (void *_self, va_list * app))
{
  struct RefObject *self = O_CAST(_self, RefObject());
  self = O_SUPER->ctor(self, app);
  self->ref_count = 0;
  /* Add self to auto release pool */
  O_CALL(current_release_pool, add, self);
  return self;
}

O_IMPLEMENT(RefObject, void *, dtor, (void *_self))
{
  struct RefObject *self = O_CAST(_self, RefObject());
#ifdef O_DEBUG
  assertTrue (self->ref_count <= 0, "Do not delete a RefObject with references (%d) to it.", self->ref_count);
#endif
  if (self->ref_count <= 0)
    {
      /* Remove self from auto release pool */
      O_CALL(current_release_pool, remove, self);
    }
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(RefObject, void *, retain, (void *_self))
{
  struct RefObject *self = O_CAST(_self, RefObject());
  if (self->ref_count <= 0)
    {
      /* Remove self from auto release pool */
      O_CALL(current_release_pool, remove, self);
      self->ref_count = 0;
    }
  self->ref_count++;
  return self;
}

O_IMPLEMENT(RefObject, void *, release, (void *_self))
{
  struct RefObject *self = O_CAST(_self, RefObject());
  self->ref_count--;
  if (self->ref_count == 0)
    {
      O_CALL(self, delete);
      self = NULL;
    }
  return self;
}

O_IMPLEMENT(RefObject, void *, auto_release, (void *_self))
{
  struct RefObject *self = O_CAST(_self, RefObject());
  self->ref_count--;
  if (self->ref_count == 0)
    {
      /* TODO add to auto release pool */
      O_CALL(current_release_pool, add, self);
    }
  return self;
}

O_OBJECT(RefObject, Object);
O_OBJECT_METHOD(RefObject, ctor);
O_OBJECT_METHOD(RefObject, dtor);
O_OBJECT_METHOD(RefObject, retain);
O_OBJECT_METHOD(RefObject, release);
O_OBJECT_METHOD(RefObject, auto_release);
O_END_OBJECT
