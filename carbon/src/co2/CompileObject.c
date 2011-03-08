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
#include "co2/CompileObject.h"

#define O_SUPER RefObject()

O_IMPLEMENT (CompileObject, void *, ctor, (void *_self, va_list * app))
{
  struct CompileObject *self = O_CAST (_self, CompileObject ());
  self = O_SUPER->ctor (self, app);
  return self;
}

O_IMPLEMENT (CompileObject, void *, dtor, (void *_self))
{
  struct CompileObject *self = O_CAST (_self, CompileObject ());
  return O_SUPER->dtor (self);
}

/* filter methods */
int
type_filter (void *_member, va_list * app)
{
  return o_is_of (_member, va_arg (*app, void *));
}

int
not_type_filter (void *_member, va_list * app)
{
  return !type_filter (_member, app);;
}

void
CompileObject_type_check (void *_object)
{
  struct CompileObject *object = O_CAST (_object, CompileObject ());
  O_CALL (object, type_check);
}

void
CompileObject_generate (void *_object)
{
  struct CompileObject *object = O_CAST (_object, CompileObject ());
  O_CALL (object, generate);
}

void
accept (void *_object, va_list *app)
{
  struct CompileObject *object = O_CAST (_object, CompileObject ());
  struct BaseCompileObjectVisitor *visitor = O_GET_ARG (BaseCompileObjectVisitor);
  O_CALL (object, accept, visitor);
}

O_IMPLEMENT (CompileObject, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct CompileObject *self = O_CAST (_self, CompileObject ());
  O_CALL (visitor, visit, self);
}

O_OBJECT (CompileObject, RefObject);
O_OBJECT_METHOD (CompileObject, ctor);
O_OBJECT_METHOD (CompileObject, dtor);
O_OBJECT_METHOD (CompileObject, accept);
O_END_OBJECT
