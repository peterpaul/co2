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
#include "co2/DeleteStatement.h"
#include "co2/Expression.h"
#include "co2/ObjectType.h"
#include "co2/io.h"

#define O_SUPER Statement()

O_IMPLEMENT (DeleteStatement, void *, ctor, (void *_self, va_list * app))
{
  struct DeleteStatement *self = O_CAST (_self, DeleteStatement ());
  self = O_SUPER->ctor (self, app);
  self->expr = O_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT (DeleteStatement, void *, dtor, (void *_self))
{
  struct DeleteStatement *self = O_CAST (_self, DeleteStatement ());
  O_CALL (self->expr, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (DeleteStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct DeleteStatement *self = O_CAST (_self, DeleteStatement ());
  O_CALL (self->expr, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (DeleteStatement, void, generate, (void *_self))
{
  struct DeleteStatement *self = O_CAST (_self, DeleteStatement ());
  /* TODO when self->expr->type == ArrayType, then free, else delete object */
  if (self->expr->type && o_is_of (self->expr->type, ObjectType ()))
    {
      fprintf (out, "O_CALL (");
      O_CALL (self->expr, generate);
      fprintf (out, ", delete);\n");
    }
  else
    {
      fprintf (out, "free (");
      O_CALL (self->expr, generate);
      fprintf (out, ");\n");
    }
}

O_IMPLEMENT (DeleteStatement, void, type_check, (void *_self))
{
  struct DeleteStatement *self = O_CAST (_self, DeleteStatement ());
  O_CALL (self->expr, type_check);
}

O_OBJECT (DeleteStatement, Statement);
O_OBJECT_METHOD (DeleteStatement, ctor);
O_OBJECT_METHOD (DeleteStatement, dtor);
O_OBJECT_METHOD (DeleteStatement, accept);
O_OBJECT_METHOD (DeleteStatement, generate);
O_OBJECT_METHOD (DeleteStatement, type_check);
O_END_OBJECT
