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
#include "co2/NestedExpression.h"
#include "co2/Type.h"
#include "co2/io.h"

#define O_SUPER Expression()

O_IMPLEMENT (NestedExpression, void *, ctor, (void *_self, va_list * app))
{
  struct NestedExpression *self = O_CAST (_self, NestedExpression ());
  self = O_SUPER->ctor (self, app);
  self->expr = O_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT (NestedExpression, void *, dtor, (void *_self))
{
  struct NestedExpression *self = O_CAST (_self, NestedExpression ());
  O_CALL (self->expr, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (NestedExpression, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct NestedExpression *self = O_CAST (_self, NestedExpression ());
  O_CALL (self->expr, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (NestedExpression, void, type_check, (void *_self))
{
  struct NestedExpression *self = O_CAST (_self, NestedExpression ());
  O_CALL (self->expr, type_check);
  self->type = O_CALL (self->expr->type, retain);
}

O_IMPLEMENT (NestedExpression, void, generate, (void *_self))
{
  struct NestedExpression *self = O_CAST (_self, NestedExpression ());
  fprintf (out, "(");
  O_CALL (self->expr, generate);
  fprintf (out, ")");
}

O_OBJECT (NestedExpression, Expression);
O_OBJECT_METHOD (NestedExpression, ctor);
O_OBJECT_METHOD (NestedExpression, dtor);
O_OBJECT_METHOD (NestedExpression, accept);
O_OBJECT_METHOD (NestedExpression, type_check);
O_OBJECT_METHOD (NestedExpression, generate);
O_END_OBJECT
