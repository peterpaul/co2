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
#include "co2/ConditionalExpression.h"
#include "co2/ObjectType.h"
#include "co2/ClassDeclaration.h"
#include "co2/io.h"

#define O_SUPER Expression()

O_IMPLEMENT(ConditionalExpression, void *, ctor, (void *_self, va_list *app))
{
  struct ConditionalExpression * self = O_CAST(_self, ConditionalExpression());
  self = O_SUPER->ctor(self, app);
  self->condition = O_RETAIN_ARG (Expression);
  self->then_expr = O_RETAIN_ARG (Expression);
  self->else_expr = O_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT(ConditionalExpression, void *, dtor, (void *_self))
{
  struct ConditionalExpression *self = O_CAST(_self, ConditionalExpression());
  O_CALL (self->condition, release);
  O_CALL (self->then_expr, release);
  O_CALL (self->else_expr, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(ConditionalExpression, void, type_check, (void *_self))
{
  struct ConditionalExpression *self = O_CAST(_self, ConditionalExpression());
  O_CALL (self->condition, type_check);
  O_CALL (self->then_expr, type_check);
  O_CALL (self->else_expr, type_check);
  bool then_compat_else = O_CALL (self->then_expr->type, is_compatible, self->else_expr->type);
  bool else_compat_then = O_CALL (self->else_expr->type, is_compatible, self->then_expr->type);
  if (then_compat_else)
    {
      self->type = O_CALL (self->then_expr->type, retain);
    }
  else if (else_compat_then)
    {
      self->type = O_CALL (self->else_expr->type, retain);
    }
  else
    {
      if (o_is_of (self->then_expr->type, ObjectType ())
	  && o_is_of (self->else_expr->type, ObjectType ()))
	{
	  struct ObjectType * then_type = O_CAST (self->then_expr->type, ObjectType ());
	  struct ObjectType * else_type = O_CAST (self->else_expr->type, ObjectType ());
	  if (o_is_of (then_type->decl, ClassDeclaration ())
	      && o_is_of (else_type->decl, ClassDeclaration ()))
	    {
	      struct ClassDeclaration * then_decl = O_CAST (then_type->decl, ClassDeclaration ());
	      struct ClassDeclaration * else_decl = O_CAST (else_type->decl, ClassDeclaration ());
	      struct ClassDeclaration * result = O_CALL (then_decl, find_common_base, else_decl);
	      if (result)
		{
		  self->type = O_CALL_CLASS (ObjectType (), new, result->name, result);
		  O_CALL (self->type, retain);
		  return;
		}
	    }
	}
      // check whether objecttype, and if so find common base
      O_CALL (self->then_expr->type, assert_compatible, self->else_expr->type);
      O_CALL (self->else_expr->type, assert_compatible, self->then_expr->type);
    }
}

O_IMPLEMENT(ConditionalExpression, void, generate, (void *_self))
{
  struct ConditionalExpression *self = O_CAST(_self, ConditionalExpression());
  O_CALL (self->condition, generate);
  fprintf (out, "?");
  O_CALL (self->then_expr, generate);
  fprintf (out, ":");
  O_CALL (self->else_expr, generate);
}

O_IMPLEMENT (ConditionalExpression, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct ConditionalExpression *self = O_CAST (_self, ConditionalExpression ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->condition, accept, visitor);
  O_CALL (self->then_expr, accept, visitor);
  O_CALL (self->else_expr, accept, visitor);
  O_CALL (visitor, visit, self);
  O_BRANCH_CALL (current_context, remove_last);
}

O_OBJECT(ConditionalExpression, Expression);
O_OBJECT_METHOD(ConditionalExpression, ctor);
O_OBJECT_METHOD(ConditionalExpression, dtor);
O_OBJECT_METHOD(ConditionalExpression, type_check);
O_OBJECT_METHOD(ConditionalExpression, generate);
O_OBJECT_METHOD(ConditionalExpression, accept);
O_END_OBJECT
