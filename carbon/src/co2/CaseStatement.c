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
#include "co2/CaseStatement.h"
#include "co2/Expression.h"
#include "co2/RefList.h"
#include "co2/io.h"

#define O_SUPER Statement()

O_IMPLEMENT(CaseStatement, void *, ctor, (void *_self, va_list *app))
{
  struct CaseStatement * self = O_CAST(_self, CaseStatement());
  self = O_SUPER->ctor(self, app);
  self->expr = O_BRANCH_RETAIN_ARG (Expression);
  self->statements = O_RETAIN_ARG (RefList);
  return self;
}

O_IMPLEMENT(CaseStatement, void *, dtor, (void *_self))
{
  struct CaseStatement *self = O_CAST(_self, CaseStatement());
  O_BRANCH_CALL (self->expr, release);
  O_CALL (self->statements, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT (CaseStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct CaseStatement *self = O_CAST (_self, CaseStatement ());
  O_BRANCH_CALL (self->expr, accept, visitor);
  O_CALL (self->statements, map_args, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT(CaseStatement, void, type_check, (void *_self))
{
  struct CaseStatement *self = O_CAST(_self, CaseStatement());
  O_BRANCH_CALL (current_context, add, self);
  O_BRANCH_CALL (self->expr, type_check);
  O_CALL (self->statements, map, CompileObject_type_check);
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT(CaseStatement, void, generate, (void *_self))
{
  struct CaseStatement *self = O_CAST(_self, CaseStatement());
  if (self->expr)
    {
      fprintf (out, "case ");
      O_CALL (self->expr, generate);
      fprintf (out, " :\n");
    }
  else
    {
      fprintf (out, "default:\n");
    }
  O_CALL (self->statements, map, CompileObject_generate);
}

O_OBJECT(CaseStatement, Statement);
O_OBJECT_METHOD(CaseStatement, ctor);
O_OBJECT_METHOD(CaseStatement, dtor);
O_OBJECT_METHOD(CaseStatement, accept);
O_OBJECT_METHOD(CaseStatement, type_check);
O_OBJECT_METHOD(CaseStatement, generate);
O_END_OBJECT
