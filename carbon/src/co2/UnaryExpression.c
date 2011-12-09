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
#include "co2/UnaryExpression.h"
#include "co2/Token.h"
#include "co2/io.h"
#include "co2/ArrayType.h"
#include "co2/PrimitiveType.h"
#include "grammar.h"

#define O_SUPER Expression()

O_IMPLEMENT (UnaryExpression, void *, ctor, (void *_self, va_list * app))
{
  struct UnaryExpression *self = O_CAST (_self, UnaryExpression ());
  self = O_SUPER->ctor (self, app);
  self->operator = O_BRANCH_RETAIN_ARG (Token);
  self->operand = O_BRANCH_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT (UnaryExpression, void *, dtor, (void *_self))
{
  struct UnaryExpression *self = O_CAST (_self, UnaryExpression ());
  O_BRANCH_CALL (self->operator, release);
  O_BRANCH_CALL (self->operand, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (UnaryExpression, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct UnaryExpression *self = O_CAST (_self, UnaryExpression ());
  O_BRANCH_CALL (self->operator, accept, visitor);
  O_BRANCH_CALL (self->operand, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (UnaryExpression, void, type_check, (void *_self))
{
  struct UnaryExpression *self = O_CAST (_self, UnaryExpression ());
  O_CALL (self->operand, type_check);
  switch (self->operator->type)
    {
    case '&':
      self->type = O_CALL_CLASS (ArrayType (), new, self->operand->type);
      O_CALL (self->type, retain);
      break;
    case '*':
      if (o_is_of (self->operand->type, ArrayType ()))
	{
	  struct ArrayType * array_type = O_CAST (self->operand->type, ArrayType ());
	  self->type = O_CALL (array_type->base_type, retain);
	}
      else
	{
	  error (O_CALL (self->operand->type, get_token), "Cannot dereference %c type.\n", self->operand->type->class->name);
	}
      break;
    case '!':
      {
	struct Token * int_token = O_CALL_CLASS(Token (), new_ctor, _Token_ctor_from_token, self->operator, "int", INT);
	struct Type * int_type = O_CALL_CLASS(PrimitiveType(), new, int_token);
	self->type = O_CALL (int_type, retain);
	O_BRANCH_CALL (self->operand->type, assume_compatible, int_type);
      }
      break;
    default:
      self->type = O_CALL(self->operand->type, retain);
      break;
    }
}

O_IMPLEMENT (UnaryExpression, void, generate, (void *_self))
{
  struct UnaryExpression *self = O_CAST (_self, UnaryExpression ());
  O_CALL (self->operator, generate);
  fprintf (out, " ");
  O_CALL (self->operand, generate);
}

O_OBJECT (UnaryExpression, Expression);
O_OBJECT_METHOD (UnaryExpression, ctor);
O_OBJECT_METHOD (UnaryExpression, dtor);
O_OBJECT_METHOD (UnaryExpression, accept);
O_OBJECT_METHOD (UnaryExpression, generate);
O_OBJECT_METHOD (UnaryExpression, type_check);
O_END_OBJECT
