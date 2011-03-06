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
#include "co2/VarArgExpression.h"
#include "co2/io.h"

#define O_SUPER Expression()

O_IMPLEMENT (VarArgExpression, void *, ctor, (void *_self, va_list * app))
{
  struct VarArgExpression *self = O_CAST (_self, VarArgExpression ());
  self = O_SUPER->ctor (self, app);
  self->va_arg_type = O_RETAIN_ARG (Type);
  self->va_arg_expr = O_BRANCH_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT (VarArgExpression, void *, dtor, (void *_self))
{
  struct VarArgExpression *self = O_CAST (_self, VarArgExpression ());
  O_CALL (self->va_arg_type, release);
  O_BRANCH_CALL (self->va_arg_expr, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (VarArgExpression, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct VarArgExpression *self = O_CAST (_self, VarArgExpression ());
  O_CALL (self->va_arg_type, accept, visitor);
  O_BRANCH_CALL (self->va_arg_expr, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (VarArgExpression, void, type_check, (void *_self))
{
  struct VarArgExpression *self = O_CAST (_self, VarArgExpression ());
  O_CALL (self->va_arg_type, type_check);
  O_BRANCH_CALL (self->va_arg_expr, type_check);
  self->type = O_CALL (self->va_arg_type, retain);
}

O_IMPLEMENT (VarArgExpression, void, generate, (void *_self))
{
  struct VarArgExpression *self = O_CAST (_self, VarArgExpression ());
  if (self->va_arg_expr)
    {
      fprintf (out, "va_arg(");
      O_CALL (self->va_arg_expr, generate);
      fprintf (out, ", ");
      O_CALL (self->va_arg_type, generate);
      fprintf (out, ")");
    }
  else
    {
      fprintf (out, "va_arg(ap, ");
      O_CALL (self->va_arg_type, generate);
      fprintf (out, ")");
    }
}

O_OBJECT (VarArgExpression, Expression);
O_OBJECT_METHOD (VarArgExpression, ctor);
O_OBJECT_METHOD (VarArgExpression, dtor);
O_OBJECT_METHOD (VarArgExpression, accept);
O_OBJECT_METHOD (VarArgExpression, type_check);
O_OBJECT_METHOD (VarArgExpression, generate);
O_END_OBJECT
