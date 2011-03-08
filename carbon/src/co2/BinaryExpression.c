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
#include "co2/BinaryExpression.h"
#include "co2/Token.h"
#include "co2/Type.h"
#include "co2/ObjectType.h"
#include "co2/Declaration.h"
#include "co2/ArrayType.h"
#include "co2/ObjectTypeDeclaration.h"
#include "co2/StructDeclaration.h"
#include "co2/io.h"
#include "co2/PrimitiveType.h"
#include "grammar.h"

#define O_SUPER Expression()

static bool is_condition (struct BinaryExpression *self)
{
  switch (self->operator->type)
    {
    case '>':
    case '<':
    case EQ:
    case NEQ:
    case AND:
    case OR:
    case XOR:
    case LEQ:
    case GEQ:
      return true;
    default:
      return false;
    }
}

O_IMPLEMENT (BinaryExpression, void *, ctor, (void *_self, va_list * app))
{
  struct BinaryExpression *self = O_CAST (_self, BinaryExpression ());
  self = O_SUPER->ctor (self, app);
  self->operand[0] = O_BRANCH_RETAIN_ARG (Expression);
  self->operator = O_BRANCH_RETAIN_ARG (Token);
  self->operand[1] = O_BRANCH_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT (BinaryExpression, void *, dtor, (void *_self))
{
  struct BinaryExpression *self = O_CAST (_self, BinaryExpression ());
  O_BRANCH_CALL (self->operand[0], release);
  O_BRANCH_CALL (self->operand[1], release);
  O_BRANCH_CALL (self->operator, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (BinaryExpression, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct BinaryExpression *self = O_CAST (_self, BinaryExpression ());
  O_BRANCH_CALL (self->operand[0], accept, visitor);
  O_BRANCH_CALL (self->operator, accept, visitor);
  O_BRANCH_CALL (self->operand[1], accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (BinaryExpression, void, generate, (void *_self))
{
  struct BinaryExpression *self = O_CAST (_self, BinaryExpression ());
  switch (self->operator->type)
    {
    case '.':
      {
	struct ObjectType * type = o_cast (self->operand[0]->type, ObjectType ());
	O_CALL (self->operand[0], generate);
	if (o_is_of (type->decl, StructDeclaration ()))
	  {
	    fprintf (out, ".");
	  }
	else
	  {
	    fprintf (out, "->");
	  }
	O_CALL (self->operand[1], generate_left, false);
      }
      break;
    case '[':
      O_CALL (self->operand[0], generate);
      O_CALL (self->operator, generate);
      O_CALL (self->operand[1], generate);
      fprintf (out, "]");
      break;
    case '%':
      {
	struct PrimitiveType * type = O_CAST (self->type, PrimitiveType ());
	switch (type->token->type)
	  {
	  case FLOAT:
	    fprintf (out, "fmodf (");
	    O_CALL (self->operand[0], generate);
	    fprintf (out, ", ");
	    O_CALL (self->operand[1], generate);
	    fprintf (out, ")");
	    break;
	  default:
	    O_CALL (self->operand[0], generate);
	    fprintf (out, " ");
	    O_CALL (self->operator, generate);
	    fprintf (out, " ");
	    O_CALL (self->operand[1], generate);
	    break;
	  }
      }
      break;
    case REMINDER:
      {
	struct PrimitiveType * type = O_CAST (self->type, PrimitiveType ());
	switch (type->token->type)
	  {
	  case FLOAT:
	    O_CALL (self->operand[0], generate);
	    fprintf (out, " = ");
	    fprintf (out, "fmodf (");
	    O_CALL (self->operand[0], generate);
	    fprintf (out, ", ");
	    O_CALL (self->operand[1], generate);
	    fprintf (out, ")");
	    break;
	  default:
	    O_CALL (self->operand[0], generate);
	    fprintf (out, " ");
	    O_CALL (self->operator, generate);
	    fprintf (out, " ");
	    O_CALL (self->operand[1], generate);
	    break;
	  }
      }
      break;
    case '^':
      {
	struct PrimitiveType * type = O_CAST (self->type, PrimitiveType ());
	switch (type->token->type)
	  {
	  case INT:
	    fprintf (out, "ipow (");
	  break;
	  case UNSIGNED:
	    fprintf (out, "upow (");
	    break;
	  case FLOAT:
	    fprintf (out, "powf (");
	    break;
	  }
	O_CALL (self->operand[0], generate);
	fprintf (out, ", ");
	O_CALL (self->operand[1], generate);
	fprintf (out, ")");
      }
      break;
    case POWER:
      {
	struct PrimitiveType * type = O_CAST (self->type, PrimitiveType ());
	O_CALL (self->operand[0], generate);
	fprintf (out, " = ");
	switch (type->token->type)
	  {
	  case INT:
	    fprintf (out, "ipow (");
	  break;
	  case UNSIGNED:
	    fprintf (out, "upow (");
	    break;
	  case FLOAT:
	    fprintf (out, "powf (");
	    break;
	  }
	O_CALL (self->operand[0], generate);
	fprintf (out, ", ");
	O_CALL (self->operand[1], generate);
	fprintf (out, ")");
      }
      break;
    default:
      O_CALL (self->operand[0], generate);
      fprintf (out, " ");
      O_CALL (self->operator, generate);
      fprintf (out, " ");
      O_CALL (self->operand[1], generate);
      break;
    }
}

O_IMPLEMENT (BinaryExpression, void, type_check, (void *_self))
{
  struct BinaryExpression *self = O_CAST (_self, BinaryExpression ());
  switch (self->operator-> type)
    {
    case '.':
      O_CALL (self->operand[0], type_check);
      if (!o_is_of (self->operand[0]->type, ObjectType ()))
	{
	  error (self->operator, "Expected ObjectType\n");
	  return;
	}
      struct ObjectType *object_type =
	o_cast (self->operand[0]->type, ObjectType ());
      struct ObjectTypeDeclaration *class_decl =
	o_cast (object_type->decl, ObjectTypeDeclaration ());
      O_CALL (self->operand[1], set_scope, class_decl->member_scope);
      O_CALL (self->operand[1], type_check);
      self->type = O_BRANCH_CALL (self->operand[1]->type, retain);
      break;
    case '[':
      O_CALL (self->operand[0], type_check);
      struct ArrayType *array_type =
	o_cast (self->operand[0]->type, ArrayType ());
      O_CALL (self->operand[1], type_check);
      self->type = O_CALL (array_type->base_type, retain);
      break;
    default:
      O_CALL (self->operand[0], type_check);
      O_CALL (self->operand[1], type_check);
      if (is_condition (self))
	{
	  struct Token * int_token = O_CALL_CLASS(Token (), new_ctor, _Token_ctor_from_token, self->operator, "int", INT);
	  struct Type * int_type = O_CALL_CLASS(PrimitiveType(), new, int_token);
	  self->type = O_CALL (int_type, retain);
	  O_BRANCH_CALL (self->operand[0]->type, assume_compatible, int_type);
	  O_BRANCH_CALL (self->operand[1]->type, assume_compatible, int_type);
	}
      else if (self->operand[1]->type)
	{
	  O_BRANCH_CALL (self->operand[0]->type, assert_compatible,
			 self->operand[1]->type);
	  self->type = O_BRANCH_CALL (self->operand[0]->type, retain);
	}
      break;
    }
}

O_IMPLEMENT (BinaryExpression, void, set_scope, (void *_self, void *_scope))
{
  struct BinaryExpression *self = O_CAST (_self, BinaryExpression ());
  O_CALL (self->operand[0], set_scope, _scope);
}

O_IMPLEMENT(BinaryExpression, struct Token *, get_token, (void *_self))
{
  struct BinaryExpression *self = O_CAST (_self, BinaryExpression ());
  return O_CALL (self->operand[1], get_token);
}


O_OBJECT (BinaryExpression, Expression);
O_OBJECT_METHOD (BinaryExpression, ctor);
O_OBJECT_METHOD (BinaryExpression, dtor);
O_OBJECT_METHOD (BinaryExpression, accept);
O_OBJECT_METHOD (BinaryExpression, generate);
O_OBJECT_METHOD (BinaryExpression, type_check);
O_OBJECT_METHOD (BinaryExpression, set_scope);
O_OBJECT_METHOD (BinaryExpression, get_token);
O_END_OBJECT
