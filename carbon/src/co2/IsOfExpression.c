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
#include "co2/IsOfExpression.h"
#include "co2/Token.h"
#include "co2/PrimitiveType.h"
#include "co2/io.h"
#include "grammar.h"

#define O_SUPER Expression()

O_IMPLEMENT (IsOfExpression, void *, ctor, (void *_self, va_list *app))
{
  struct IsOfExpression * self = O_CAST (_self, IsOfExpression());
  self = O_SUPER->ctor (self, app);
  self->expr_to_check = O_RETAIN_ARG (Expression);
  self->class_expr = O_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT (IsOfExpression, void *, dtor, (void *_self))
{
  struct IsOfExpression *self = O_CAST (_self, IsOfExpression());
  O_CALL (self->expr_to_check, release);
  O_CALL (self->class_expr, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (IsOfExpression, void, type_check, (void *_self))
{
  struct IsOfExpression *self = O_CAST (_self, IsOfExpression());
  O_CALL (self->expr_to_check, type_check);
  O_CALL (self->class_expr, type_check);
  struct Token * expr_token = O_CALL (self->class_expr, get_token);
  struct Token * int_token = O_CALL_CLASS(Token (), new_ctor, _Token_ctor_from_token, expr_token, "int", INT);
  self->type = O_CALL_CLASS (PrimitiveType (), new, int_token);
  
  O_CALL (self->type, retain);

}

O_IMPLEMENT (IsOfExpression, void, generate, (void *_self))
{
  struct IsOfExpression *self = O_CAST (_self, IsOfExpression());
  fprintf (out, "o_is_of (");
  O_CALL (self->expr_to_check, generate);
  fprintf (out, ", ");
  O_CALL (self->class_expr, generate);
  fprintf (out, ")");
}

O_IMPLEMENT (IsOfExpression, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct IsOfExpression *self = O_CAST (_self, IsOfExpression ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->expr_to_check, accept, visitor);
  O_CALL (self->class_expr, accept, visitor);
  O_CALL (visitor, visit, self);
  O_BRANCH_CALL (current_context, remove_last);
}

O_OBJECT (IsOfExpression, Expression);
O_OBJECT_METHOD (IsOfExpression, ctor);
O_OBJECT_METHOD (IsOfExpression, dtor);
O_OBJECT_METHOD (IsOfExpression, type_check);
O_OBJECT_METHOD (IsOfExpression, generate);
O_OBJECT_METHOD (IsOfExpression, accept);
O_END_OBJECT
