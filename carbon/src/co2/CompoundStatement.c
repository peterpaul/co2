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
#include "co2/CompoundStatement.h"
#include "co2/Declaration.h"
#include "co2/io.h"

#define O_SUPER Statement()

O_IMPLEMENT (CompoundStatement, void *, ctor, (void *_self, va_list * app))
{
  struct CompoundStatement *self = O_CAST (_self, CompoundStatement ());
  self = O_SUPER->ctor (self, app);
  self->body = O_RETAIN_ARG (RefList);
  return self;
}

O_IMPLEMENT (CompoundStatement, void *, dtor, (void *_self))
{
  struct CompoundStatement *self = O_CAST (_self, CompoundStatement ());
  O_CALL (self->body, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (CompoundStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct CompoundStatement *self = O_CAST (_self, CompoundStatement ());
  O_CALL (self->body, map_args, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (CompoundStatement, void, generate, (void *_self))
{
  struct CompoundStatement *self = O_CAST (_self, CompoundStatement ());
  fprintf (out, "{\n");
  O_CALL (self->body, map, CompileObject_generate);
  fprintf (out, "}\n");
}

O_IMPLEMENT (CompoundStatement, void, type_check, (void *_self))
{
  struct CompoundStatement *self = O_CAST (_self, CompoundStatement ());
  O_CALL (self->body, map, CompileObject_type_check);
}

O_OBJECT (CompoundStatement, Statement);
O_OBJECT_METHOD (CompoundStatement, ctor);
O_OBJECT_METHOD (CompoundStatement, dtor);
O_OBJECT_METHOD (CompoundStatement, accept);
O_OBJECT_METHOD (CompoundStatement, generate);
O_OBJECT_METHOD (CompoundStatement, type_check);
O_END_OBJECT
