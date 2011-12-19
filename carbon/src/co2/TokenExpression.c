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
#include "co2/Declaration.h"
#include "co2/IScope.h"
#include "co2/TokenExpression.h"
#include "grammar.h"
#include "co2/PrimitiveType.h"
#include "co2/ArrayType.h"
#include "co2/FunctionType.h"
#include "co2/ObjectType.h"
#include "co2/ClassDeclaration.h"
#include "co2/Token.h"
#include "grammar.h"
#include "co2/io.h"

#define O_SUPER Expression()

O_IMPLEMENT (TokenExpression, void *, ctor, (void *_self, va_list * app))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  self = O_SUPER->ctor (self, app);
  self->token = O_RETAIN_ARG (Token);
  if (self->token->type == IDENTIFIER || self->token->type == TYPE_IDENTIFIER || self->token->type == CLASS)
    {
      self->scope = current_scope;
    }
  self->check_global_scope = true;

  // TODO this is not a final solution, as it might not resolve the right var
  if (O_BRANCH_CALL_IF (IScope, current_scope, exists, self->token))
    {
      self->decl = O_CALL_IF (IScope, current_scope, lookup, self->token);
      O_CALL (self->decl, retain);
    }
  return self;
}

O_IMPLEMENT (TokenExpression, void *, dtor, (void *_self))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  O_CALL (self->token, release);
  O_BRANCH_CALL (self->decl, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (TokenExpression, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  O_CALL (self->token, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (TokenExpression, void, generate_left, (void *_self, bool left))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  if (self->token->type == VA_ARG)
    {
      fprintf (out, "ap");
      return;
    }
  if (left && self->decl && self->decl->class_decl)
    {
      fprintf (out, "self->");
    }
  O_CALL (self->token, generate);
  if (self->token->type == TYPE_IDENTIFIER)
    {
      fprintf (out, " ()");
    }
}

O_IMPLEMENT (TokenExpression, void, set_scope, (void *_self, void *_scope))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  self->scope = O_CAST_INTERFACE (_scope, IScope ());
  self->check_global_scope = false;
}

O_IMPLEMENT (TokenExpression, void, type_check, (void *_self))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  if (!self->decl && (self->token->type == IDENTIFIER || self->token->type == TYPE_IDENTIFIER || self->token->type == CLASS))
    {
      if (self->scope && O_CALL_IF (IScope, self->scope, exists, self->token))
	{
	  self->decl = O_CALL_IF (IScope, self->scope, lookup, self->token);
	}
      if (!self->decl && O_CALL_IF (IScope, global_scope, exists, self->token))
	{
	  self->decl = O_CALL_IF (IScope, global_scope, lookup, self->token);
	}
      O_BRANCH_CALL (self->decl, retain);
    }
  switch (self->token->type)
    {
    case TYPE_IDENTIFIER:
      warning (self->token, "Handling TokenExpression: '%s'\n",
	       self->token->name->data);
      if (!self->decl || !self->check_global_scope)
	{
	  O_CALL (self, lookup);
	}
      if (!self->decl)
	{
	  return;
	}
      self->type = O_CALL_CLASS (ObjectType (), new, self->token, self->decl);
      self->type = O_CALL (self->type, retain);
      break;
    case CLASS:
    case IDENTIFIER:
      if (!self->decl || !self->check_global_scope)
	{
	  O_CALL (self, lookup);
	}
      if (!self->decl)
	{
	  return;
	}
      self->type = O_CALL (O_CALL (self->decl->type, get_declared_type), retain);
      break;
    case SELF:
      {
	struct ClassDeclaration *class_decl =
	  O_BRANCH_CALL (current_context, find, ClassDeclaration ());
	self->type =
	  O_CALL_CLASS (ObjectType (), new, class_decl->name, class_decl);
	O_CALL (self->type, retain);
      }
      break;
    case INT_CONSTANT:
      {
	struct Token *token =
	  O_CALL_CLASS (Token (), new_ctor, _Token_ctor_from_token, self->token, "int", INT);
	self->type = O_CALL_CLASS (PrimitiveType (), new, token);
	O_CALL (self->type, retain);
      }
      break;
    case STRING_CONSTANT:
      {
	struct Token *token =
	  O_CALL_CLASS (Token (), new_ctor, _Token_ctor_from_token, self->token, "char", CHAR);
	struct Type *base_type = O_CALL_CLASS (PrimitiveType (), new, token);
	self->type = O_CALL_CLASS (ArrayType (), new, base_type);
	O_CALL (self->type, retain);
      }
      break;
    case CHAR_CONSTANT:
      {
	struct Token *token =
	  O_CALL_CLASS (Token (), new_ctor, _Token_ctor_from_token, self->token, "char", CHAR);
	self->type = O_CALL_CLASS (PrimitiveType (), new, token);
	O_CALL (self->type, retain);
      }
      break;
    case FLOAT_CONSTANT:
      {
	struct Token *token =
	  O_CALL_CLASS (Token (), new_ctor, _Token_ctor_from_token, self->token, "float", FLOAT);
	self->type = O_CALL_CLASS (PrimitiveType (), new, token);
	O_CALL (self->type, retain);
      }
      break;
    case VA_ARG:
      {
	struct Token *token =
	  O_CALL_CLASS (Token (), new_ctor, _Token_ctor_from_token, self->token, "va_list", VA_LIST);
	self->type = O_CALL_CLASS (PrimitiveType (), new, token);
	O_CALL (self->type, retain);
      }
      break;
    default:
      error (self->token, "Unhandled TokenExpression: '%s'\n",
	     self->token->name->data);
      break;
    }
  O_CALL (self->type, type_check);
}

O_IMPLEMENT (TokenExpression, void, lookup, (void *_self))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  O_BRANCH_CALL (self->decl, release);
  if (self->check_global_scope)
    {
      if (O_CALL_IF (IScope, self->scope, exists, self->token))
	{
	  self->decl = O_CALL_IF (IScope, self->scope, lookup, self->token);
	}
      else
	{
	  self->decl = O_CALL_IF (IScope, global_scope, lookup, self->token);
	}
    }
  else
    {
      self->decl = O_CALL_IF (IScope, self->scope, lookup, self->token);
    }
  O_BRANCH_CALL (self->decl, retain);
}

O_IMPLEMENT(TokenExpression, struct Token *, get_token, (void *_self))
{
  struct TokenExpression *self = O_CAST (_self, TokenExpression ());
  return self->token;
}

O_OBJECT (TokenExpression, Expression);
O_OBJECT_METHOD (TokenExpression, ctor);
O_OBJECT_METHOD (TokenExpression, dtor);
O_OBJECT_METHOD (TokenExpression, accept);
O_OBJECT_METHOD (TokenExpression, generate_left);
O_OBJECT_METHOD (TokenExpression, type_check);
O_OBJECT_METHOD (TokenExpression, set_scope);
O_OBJECT_METHOD (TokenExpression, lookup);
O_OBJECT_METHOD (TokenExpression, get_token);
O_END_OBJECT
