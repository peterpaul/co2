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
#include "co2/HelloWorldVisitor.h"
#include "co2/CompileObject.h"

#define O_SUPER BaseCompileObjectVisitor()

O_IMPLEMENT(HelloWorldVisitor, void *, ctor, (void *_self, va_list *app))
{
  struct HelloWorldVisitor * self = O_CAST(_self, HelloWorldVisitor());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT(HelloWorldVisitor, void *, dtor, (void *_self))
{
  struct HelloWorldVisitor *self = O_CAST(_self, HelloWorldVisitor());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_IMPLEMENT_IF(HelloWorldVisitor, void, visit, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *self = O_CAST(_self, BaseCompileObjectVisitor());
  struct CompileObject *object = O_CAST(_object, CompileObject ());
  fprintf (stdout, "visit: %s\n", object->class->name);
}

O_OBJECT(HelloWorldVisitor, BaseCompileObjectVisitor);
O_OBJECT_METHOD(HelloWorldVisitor, ctor);
O_OBJECT_METHOD(HelloWorldVisitor, dtor);
O_OBJECT_METHOD(HelloWorldVisitor, visit);
O_OBJECT_IF(CompileObjectVisitor);
O_OBJECT_IF_METHOD(HelloWorldVisitor, visit);
O_OBJECT_IF_END
O_END_OBJECT
