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
#include "co2/SuperExpression.h"
#include "co2/Token.h"
#include "co2/FunctionDeclaration.h"
#include "co2/FunctionType.h"
#include "co2/ClassDeclaration.h"
#include "co2/ConstructorDeclaration.h"
#include "co2/ArgumentDeclaration.h"
#include "co2/Type.h"
#include "grammar.h"
#include "co2/io.h"

#define O_SUPER Expression()

O_IMPLEMENT (SuperExpression, void *, ctor, (void *_self, va_list * app))
{
  struct SuperExpression *self = O_CAST (_self, SuperExpression ());
  self = O_SUPER->ctor (self, app);
  self->super = O_BRANCH_RETAIN_ARG (Token);
  self->ctor_name = O_BRANCH_RETAIN_ARG (Token);
  self->actual_arguments = O_BRANCH_RETAIN_ARG (RefList);
  return self;
}

O_IMPLEMENT (SuperExpression, void *, dtor, (void *_self))
{
  struct SuperExpression *self = O_CAST (_self, SuperExpression ());
  O_BRANCH_CALL (self->super, release);
  O_BRANCH_CALL (self->ctor_name, release);
  O_BRANCH_CALL (self->actual_arguments, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (SuperExpression, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct SuperExpression *self = O_CAST (_self, SuperExpression ());
  O_BRANCH_CALL (self->super, accept, visitor);
  O_BRANCH_CALL (self->ctor_name, accept, visitor);
  O_BRANCH_CALL (self->actual_arguments, map_args, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (SuperExpression, void, type_check, (void *_self))
{
  struct SuperExpression *self = O_CAST (_self, SuperExpression ());
  /* get class context */
  self->class_context = O_BRANCH_CALL (current_context, find, ClassDeclaration ());
  /* get method context */
  self->method_context =
    O_BRANCH_CALL (current_context, find, ConstructorDeclaration ());
  if (self->method_context == NULL)
    {
      self->method_context =
	O_BRANCH_CALL (current_context, find, FunctionDeclaration ());
    }

  if (self->class_context->superclass == NULL)
    {
      error(self->super, "%s has no superclass.\n", self->class_context->name->name->data);
      return;
    }

  struct ClassDeclaration *super =
    (struct ClassDeclaration *) O_CALL_IF (IScope, global_scope, lookup,
					self->class_context->superclass);

  if (o_is_of (self->method_context, ConstructorDeclaration ()))
    {
      struct Token *ctor_name = self->ctor_name;
      if (ctor_name == NULL)
	{
	  ctor_name =
o	    O_CALL_CLASS (Token (), new_ctor, _Token_ctor_from_token, self->super, "ctor", IDENTIFIER);
	}
      O_CALL (ctor_name, retain);
      struct ConstructorDeclaration *ctor_decl =
	(struct ConstructorDeclaration *) O_CALL_IF (IScope, super->member_scope, lookup, ctor_name);

      if (self->actual_arguments->length <
	  ctor_decl->formal_arguments->length)
	{
	  error (self->super, "'%s' needs %d arguments, but got %d.\n",
		 ctor_decl->name->name->data,
		 ctor_decl->formal_arguments->length,
		 self->actual_arguments->length);
	  return;
	}
      int i;
      for (i = 0; i < ctor_decl->formal_arguments->length; i++)
	{
	  struct ArgumentDeclaration *arg_decl =
	    O_CALL (ctor_decl->formal_arguments, get, i);
	  struct Expression *arg_expr =
	    O_CALL (self->actual_arguments, get, i);
	  O_CALL (arg_expr, type_check);
	  O_CALL (arg_decl->type, assert_compatible, arg_expr->type);
	}
      O_CALL (ctor_name, release);
    }
  else if (o_is_of (self->method_context, FunctionDeclaration ()))
    {
      struct FunctionDeclaration *function_decl =
	(struct FunctionDeclaration *) O_CALL_IF (IScope, super->member_scope, lookup, self->method_context->name);

      if (self->actual_arguments->length <
	  function_decl->formal_arguments->length)
	{
	  error (self->super, "'%s' needs %d arguments, but got %d.\n",
		 function_decl->name->name->data,
		 function_decl->formal_arguments->length,
		 self->actual_arguments->length);
	  return;
	}
      int i;
      for (i = 0; i < function_decl->formal_arguments->length; i++)
	{
	  struct ArgumentDeclaration *arg_decl =
	    O_CALL (function_decl->formal_arguments, get, i);
	  struct Expression *arg_expr =
	    O_CALL (self->actual_arguments, get, i);
	  O_CALL (arg_expr, type_check);
	  O_CALL (arg_decl->type, assert_compatible, arg_expr->type);
	}
      struct FunctionType * function_type = o_cast (function_decl->type, FunctionType ());
      self->type = function_type->return_type;
    }
  else
    {
      error (self->super, "No context found for super keyword.\n");
    }
}

O_IMPLEMENT (SuperExpression, void, generate, (void *_self))
{
  struct SuperExpression *self = O_CAST (_self, SuperExpression ());

  struct ClassDeclaration *class_decl = O_CALL (current_context, find, ClassDeclaration ());
  if (o_is_of (self->method_context, ConstructorDeclaration ()))
    {
      if (self->ctor_name == NULL)
	{
	  bool is_first_arg = false;
	  fprintf (out, "self = o_super_ctor (self, ");
	  O_CALL (class_decl->superclass, generate);
	  fprintf (out, " ()");
	  O_CALL (self->actual_arguments, map_args,
		  Expression_generate_actual_argument, &is_first_arg);
	  fprintf (out, ")");
	}
      else
	{
	  bool is_first_arg = false;
	  fprintf (out, "self = o_super_ctor_named (self, _");
	  O_CALL (self->class_context->superclass, generate);
	  fprintf (out, "_ctor_");
	  O_CALL (self->ctor_name, generate);
	  O_CALL (self->actual_arguments, map_args,
		  Expression_generate_actual_argument, &is_first_arg);
	  fprintf (out, ")");
	}
    }
  else
    {
      bool is_first_arg = false;
      O_CALL (class_decl->superclass, generate);
      fprintf (out, " ()->");
      O_CALL (self->method_context->name, generate);
      fprintf (out, " (self");
      O_CALL (self->actual_arguments, map_args,
	      Expression_generate_actual_argument, &is_first_arg);
      fprintf (out, ")");
    }
}

O_OBJECT (SuperExpression, Expression);
O_OBJECT_METHOD (SuperExpression, ctor);
O_OBJECT_METHOD (SuperExpression, dtor);
O_OBJECT_METHOD (SuperExpression, accept);
O_OBJECT_METHOD (SuperExpression, generate);
O_OBJECT_METHOD (SuperExpression, type_check);
O_END_OBJECT
