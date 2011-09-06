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
#include "co2/SwitchStatement.h"
#include "co2/Expression.h"
#include "co2/RefList.h"
#include "co2/io.h"

#define O_SUPER Statement()

O_IMPLEMENT(SwitchStatement, void *, ctor, (void *_self, va_list *app))
{
  struct SwitchStatement * self = O_CAST(_self, SwitchStatement());
  self = O_SUPER->ctor(self, app);
  self->expr = O_RETAIN_ARG (Expression);
  self->cases = O_RETAIN_ARG (RefList);
  return self;
}

O_IMPLEMENT(SwitchStatement, void *, dtor, (void *_self))
{
  struct SwitchStatement *self = O_CAST(_self, SwitchStatement());
  O_CALL (self->expr, release);
  O_CALL (self->cases, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT (SwitchStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct SwitchStatement *self = O_CAST (_self, SwitchStatement ());
  O_CALL (self->expr, accept, visitor);
  O_CALL (self->cases, map_args, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT(SwitchStatement, void, type_check, (void *_self))
{
  struct SwitchStatement *self = O_CAST(_self, SwitchStatement());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->expr, type_check);
  O_CALL (self->cases, map, CompileObject_type_check);
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT(SwitchStatement, void, generate, (void *_self))
{
  struct SwitchStatement *self = O_CAST(_self, SwitchStatement());
  fprintf (out, "switch (");
  O_CALL (self->expr, generate);
  fprintf (out, ") {\n");

  O_CALL (self->cases, map, CompileObject_generate);

  fprintf (out, "}\n");
}

O_OBJECT(SwitchStatement, Statement);
O_OBJECT_METHOD(SwitchStatement, ctor);
O_OBJECT_METHOD(SwitchStatement, dtor);
O_OBJECT_METHOD(SwitchStatement, accept);
O_OBJECT_METHOD(SwitchStatement, type_check);
O_OBJECT_METHOD(SwitchStatement, generate);
O_END_OBJECT
