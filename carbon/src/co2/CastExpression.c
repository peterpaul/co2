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
#include "co2/CastExpression.h"
#include "co2/Type.h"
#include "co2/io.h"
#include "co2/ObjectType.h"
#include "co2/Token.h"
#include "co2/ClassDeclaration.h"
#include "co2/InterfaceDeclaration.h"

#define O_SUPER Expression()

O_IMPLEMENT(CastExpression, void *, ctor, (void *_self, va_list *app))
{
  struct CastExpression * self = O_CAST(_self, CastExpression());
  self = O_SUPER->ctor(self, app);
  self->cast_type = O_RETAIN_ARG (Type);
  self->expression = O_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT(CastExpression, void *, dtor, (void *_self))
{
  struct CastExpression *self = O_CAST(_self, CastExpression());
  O_CALL(self->cast_type, retain);
  O_CALL(self->expression, retain);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT (CastExpression, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct CastExpression *self = O_CAST (_self, CastExpression ());
  O_CALL (self->cast_type, accept, visitor);
  O_CALL (self->expression, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT(CastExpression, void, type_check, (void *_self))
{
  struct CastExpression *self = O_CAST(_self, CastExpression());
  O_CALL (self->expression, type_check);
  if (!self->expression->type) return;
  O_CALL (self->cast_type, type_check);
  
  if (o_is_of (self->cast_type, ObjectType ()) && o_is_of (self->expression->type, ObjectType()))
    {
      if (!O_CALL (self->cast_type, is_compatible, self->expression->type)
&& !O_CALL (self->expression->type, is_compatible, self->cast_type))
	{
	  struct Token *expr_token = O_CALL(self->expression->type, get_token);
	  struct String *expr_string = O_CALL (self->expression->type, to_string);
	  struct String *type_string = O_CALL (self->cast_type, to_string);
	  warning (expr_token, "casting possibly incompatible types %s to %s\n", expr_string->data, type_string->data);
	  O_CALL (expr_string, delete);
	  O_CALL (type_string, delete);
	}
    }
  else
    {
      if (!O_CALL (self->cast_type, is_compatible, self->expression->type))
	{
	  struct Token *expr_token = O_CALL(self->expression->type, get_token);
	  struct String *expr_string = O_CALL (self->expression->type, to_string);
	  struct String *type_string = O_CALL (self->cast_type, to_string);
	  warning (expr_token, "casting possibly incompatible types %s to %s\n", expr_string->data, type_string->data);
	  O_CALL (expr_string, delete);
	  O_CALL (type_string, delete);
	}
    }
  self->type = O_CALL(self->cast_type, retain);
}

O_IMPLEMENT(CastExpression, void, generate, (void *_self))
{
  struct CastExpression *self = O_CAST(_self, CastExpression());
  fprintf(out, "(");
  fprintf(out, "(");
  O_CALL(self->cast_type, generate);
  fprintf(out, ") ");
  if (o_is_of(self->cast_type, ObjectType())) 
    {
      struct ObjectType * object_type = O_CAST(self->cast_type, ObjectType());
      if (o_is_of(object_type->decl, ClassDeclaration()))
	{
	  fprintf(out, "O_BRANCH_CAST (");
	  O_CALL(self->expression, generate);
	  fprintf(out, ", ");
	  O_CALL(object_type->token, generate);
	  fprintf(out, " ())");
	}
      else if (o_is_of(object_type->decl, InterfaceDeclaration()))
	{
	  fprintf(out, "O_BRANCH_CAST_INTERFACE (");
	  O_CALL(self->expression, generate);
	  fprintf(out, ", ");
	  O_CALL(object_type->token, generate);
	  fprintf(out, " ())");
	}
      else
	{
	  O_CALL(self->expression, generate);
	}
    }
  else
    {
      O_CALL(self->expression, generate);
    }
  fprintf(out, ")");
}

O_OBJECT(CastExpression, Expression);
O_OBJECT_METHOD(CastExpression, ctor);
O_OBJECT_METHOD(CastExpression, dtor);
O_OBJECT_METHOD(CastExpression, accept);
O_OBJECT_METHOD(CastExpression, type_check);
O_OBJECT_METHOD(CastExpression, generate);
O_END_OBJECT
