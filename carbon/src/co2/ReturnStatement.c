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
#include "co2/ReturnStatement.h"
#include "co2/FunctionType.h"
#include "co2/TryStatement.h"
#include "co2/CatchStatement.h"
#include "co2/FinallyStatement.h"
#include "co2/io.h"

#define O_SUPER Statement()

O_IMPLEMENT (ReturnStatement, void *, ctor, (void *_self, va_list * app))
{
  struct ReturnStatement *self = O_CAST (_self, ReturnStatement ());
  self = O_SUPER->ctor (self, app);
  self->expr = O_BRANCH_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT (ReturnStatement, void *, dtor, (void *_self))
{
  struct ReturnStatement *self = O_CAST (_self, ReturnStatement ());
  O_BRANCH_CALL (self->expr, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (ReturnStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct ReturnStatement *self = O_CAST (_self, ReturnStatement ());
  O_BRANCH_CALL (self->expr, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (ReturnStatement, void, generate, (void *_self))
{
  struct ReturnStatement *self = O_CAST (_self, ReturnStatement ());
  struct FunctionType *function_type = o_cast (self->function_context->type, FunctionType ());

  if (!self->try_context && !function_type->has_var_args)
    {
      fprintf (out, "return ");
      O_BRANCH_CALL (self->expr, generate);
      fprintf (out, ";\n");
      return;
    }

  fprintf (out, "{\n");
  if (self->expr)
    {
      O_CALL (self->expr->type, generate);
      fprintf (out, " __return_value = ");
      O_CALL (self->expr, generate);
      fprintf (out, ";\n");
    }

  if (self->try_context)
    {
      fprintf (out, "ex_pop ();\n");
      if (self->try_context->finally_clause && !self->finally_context)
	{
	  fprintf (out, "do_finally;\n");
	}
    }

  if (function_type->has_var_args)
    {
      fprintf (out, "va_end (ap);\n");
    }

  fprintf (out, "return ");
  if (self->expr)
    {
      fprintf (out, "__return_value");
    }
  fprintf (out, ";\n");
  
  fprintf (out, "}\n");
}

O_IMPLEMENT (ReturnStatement, void, type_check, (void *_self))
{
  struct ReturnStatement *self = O_CAST (_self, ReturnStatement ());
  O_BRANCH_CALL (self->expr, type_check);

  self->function_context = O_BRANCH_CALL (current_context, find, FunctionDeclaration ());
  struct FunctionType *function_type = o_cast (self->function_context->type, FunctionType ());
  if (self->expr && self->expr->type)
    {
      O_CALL (function_type->return_type, assert_compatible, self->expr->type);
    }

  self->try_context = O_BRANCH_CALL (current_context, find, TryStatement ());
  self->catch_context = O_BRANCH_CALL (current_context, find, CatchStatement ());
  self->finally_context = O_BRANCH_CALL (current_context, find, FinallyStatement ());
}

O_OBJECT (ReturnStatement, Statement);
O_OBJECT_METHOD (ReturnStatement, ctor);
O_OBJECT_METHOD (ReturnStatement, dtor);
O_OBJECT_METHOD (ReturnStatement, accept);
O_OBJECT_METHOD (ReturnStatement, generate);
O_OBJECT_METHOD (ReturnStatement, type_check);
O_END_OBJECT
