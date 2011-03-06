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
#include "co2/ThrowStatement.h"
#include "co2/TryStatement.h"
#include "co2/CatchStatement.h"
#include "co2/FinallyStatement.h"
#include "co2/io.h"

#define O_SUPER Statement()

O_IMPLEMENT(ThrowStatement, void *, ctor, (void *_self, va_list *app))
{
  struct ThrowStatement * self = O_CAST(_self, ThrowStatement());
  self = O_SUPER->ctor(self, app);
  self->expr = O_RETAIN_ARG (Expression);
  return self;
}

O_IMPLEMENT(ThrowStatement, void *, dtor, (void *_self))
{
  struct ThrowStatement *self = O_CAST(_self, ThrowStatement());
  O_CALL (self->expr, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT (ThrowStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct ThrowStatement *self = O_CAST (_self, ThrowStatement ());
  O_BRANCH_CALL (self->expr, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT(ThrowStatement, void, type_check, (void *_self))
{
  struct ThrowStatement *self = O_CAST(_self, ThrowStatement());
  self->catch_context = O_BRANCH_CALL (current_context, find, CatchStatement ());
  self->try_context = O_BRANCH_CALL (current_context, find, TryStatement ());
  self->finally_context = O_BRANCH_CALL (current_context, find, FinallyStatement ());
  O_CALL (self->expr, type_check);
}

O_IMPLEMENT(ThrowStatement, void, generate, (void *_self))
{
  struct ThrowStatement *self = O_CAST(_self, ThrowStatement());
  if (self->catch_context)
    {
      fprintf (out, "ex_pop ();\n");
      if (self->try_context->finally_clause && !self->finally_context)
	{
	  fprintf (out, "do_finally;\n");
	}
    }
  if (self->finally_context)
    {
      fprintf (out, "ex_pop ();\n");
    }
  fprintf (out, "throw (1, ");
  O_CALL (self->expr, generate);
  fprintf (out, ", __FILE__, __LINE__, __PRETTY_FUNCTION__);\n");
}

O_OBJECT(ThrowStatement, Statement);
O_OBJECT_METHOD(ThrowStatement, ctor);
O_OBJECT_METHOD(ThrowStatement, dtor);
O_OBJECT_METHOD(ThrowStatement, accept);
O_OBJECT_METHOD(ThrowStatement, type_check);
O_OBJECT_METHOD(ThrowStatement, generate);
O_END_OBJECT
