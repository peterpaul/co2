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
#include "co2/IfStatement.h"
#include "co2/Expression.h"
#include "co2/Statement.h"
#include "co2/io.h"
#define O_SUPER Statement()

O_IMPLEMENT (IfStatement, void *, ctor, (void *_self, va_list * app))
{
  struct IfStatement *self = O_CAST (_self, IfStatement ());
  self = O_SUPER->ctor (self, app);
  self->condition = O_RETAIN_ARG(Expression);
  self->then_clause = O_RETAIN_ARG(Statement);
  self->else_clause = O_BRANCH_RETAIN_ARG(Statement);
  return self;
}

O_IMPLEMENT (IfStatement, void *, dtor, (void *_self))
{
  struct IfStatement *self = O_CAST (_self, IfStatement ());
  O_CALL(self->condition, release);
  O_CALL(self->then_clause, release);
  O_BRANCH_CALL(self->else_clause, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (IfStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct IfStatement *self = O_CAST (_self, IfStatement ());
  O_CALL (self->condition, accept, visitor);
  O_CALL (self->then_clause, accept, visitor);
  O_BRANCH_CALL (self->else_clause, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (IfStatement, void, type_check, (void *_self))
{
  struct IfStatement *self = O_CAST (_self, IfStatement ());
  O_CALL(self->condition, type_check);
  O_CALL(self->then_clause, type_check);
  O_BRANCH_CALL(self->else_clause, type_check);
}

O_IMPLEMENT (IfStatement, void, generate, (void *_self))
{
  struct IfStatement *self = O_CAST (_self, IfStatement ());
  fprintf(out, "if (");
  O_CALL(self->condition, generate);
  fprintf(out, ")\n");
  O_CALL(self->then_clause, generate);
  if (self->else_clause)
    {
      fprintf(out, " else ");
      O_CALL(self->else_clause, generate);
    }
}

O_OBJECT (IfStatement, Statement);
O_OBJECT_METHOD (IfStatement, ctor);
O_OBJECT_METHOD (IfStatement, dtor);
O_OBJECT_METHOD (IfStatement, accept);
O_OBJECT_METHOD (IfStatement, type_check);
O_OBJECT_METHOD (IfStatement, generate);
O_END_OBJECT
