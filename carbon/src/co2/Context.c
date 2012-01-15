/*
 * carbon Compiler and programming language for libco2
 *
 * Copyright (C) 2011 Peterpaul Taekele Klein Haneveld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "co2/Context.h"
#include "co2/CompileObject.h"

#define O_SUPER RefObject()

struct Context *current_context;

O_IMPLEMENT (Context, void *, ctor, (void *_self, va_list * app))
{
  struct Context *self = O_CAST (_self, Context ());
  self = O_SUPER->ctor (self, app);
  self->stack = O_CALL_CLASS (RefList (), new, 8, CompileObject ());
  O_CALL (self->stack, retain);
  return self;
}

O_IMPLEMENT (Context, void *, dtor, (void *_self))
{
  struct Context *self = O_CAST (_self, Context ());
  O_CALL (self->stack, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (Context, void, add, (void *_self, void *_item))
{
  struct Context *self = O_CAST (_self, Context ());
  O_CALL (self->stack, append, _item);
}

O_IMPLEMENT (Context, void *, find, (void *_self, void *_class))
{
  struct Context *self = O_CAST (_self, Context ());
  return O_CALL (self, find_nth_instance, _class, 1);
}

O_IMPLEMENT (Context, void *, find_nth_instance, (void *_self, void *_class, unsigned n))
{
  struct Context *self = O_CAST (_self, Context ());
  assertTrue (n > 0, "''n' can not be zero");
  int i;
  unsigned found_count = 0;
  for (i = self->stack->length - 1; i >= 0; i--)
    {
      void *item = O_CALL (self->stack, get, i);
      if (o_is_of (item, _class))
	{
	  found_count++;
	  if (found_count == n)
	    {
	      return item;
	    }
	}
    }
  return NULL;
}

O_IMPLEMENT (Context, void, remove_last, (void *_self))
{
  struct Context *self = O_CAST (_self, Context ());
  O_CALL (self->stack, remove);
}

O_OBJECT (Context, RefObject);
O_OBJECT_METHOD (Context, ctor);
O_OBJECT_METHOD (Context, dtor);
O_OBJECT_METHOD (Context, add);
O_OBJECT_METHOD (Context, find);
O_OBJECT_METHOD (Context, find_nth_instance);
O_OBJECT_METHOD (Context, remove_last);
O_END_OBJECT
