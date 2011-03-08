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
#include "co2/TryStatement.h"
#include "co2/CatchStatement.h"
#include "co2/ObjectType.h"
#include "co2/ArgumentDeclaration.h"
#include "co2/io.h"

#define O_SUPER Statement()

O_IMPLEMENT(TryStatement, void *, ctor, (void *_self, va_list *app))
{
  struct TryStatement * self = O_CAST(_self, TryStatement());
  self = O_SUPER->ctor(self, app);
  self->try_clause = O_RETAIN_ARG (Statement);
  self->catch_clause = O_RETAIN_ARG (RefList);
  self->finally_clause = O_BRANCH_RETAIN_ARG (Statement);
  return self;
}

O_IMPLEMENT(TryStatement, void *, dtor, (void *_self))
{
  struct TryStatement *self = O_CAST(_self, TryStatement());
  O_CALL (self->try_clause, release);
  O_CALL (self->catch_clause, release);
  O_BRANCH_CALL (self->finally_clause, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT (TryStatement, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct TryStatement *self = O_CAST (_self, TryStatement ());
  O_CALL (self->try_clause, accept, visitor);
  O_CALL (self->catch_clause, map_args, accept, visitor);
  O_BRANCH_CALL (self->finally_clause, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT(TryStatement, void, type_check, (void *_self))
{
  struct TryStatement *self = O_CAST(_self, TryStatement());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->try_clause, type_check);
  O_CALL (self->catch_clause, map, CompileObject_type_check);
  O_BRANCH_CALL (self->finally_clause, type_check);
  O_BRANCH_CALL (current_context, remove_last);
}

static void TryStatement_generate_catch(void *_catch, va_list *app)
{
  struct CatchStatement *catch = O_CAST (_catch, CatchStatement ());
  struct TryStatement *self = O_RETAIN_ARG (TryStatement);

  struct ObjectType *type = O_CAST (catch->argument->type, ObjectType ());
  fprintf (out, "catch (%s ()) {\n", type->token->name->data);
  O_CALL (type, generate);
  O_CALL (catch->argument->name, generate);
  fprintf (out, " = (");
  O_CALL (type, generate);  
  fprintf (out, ") ex_stack->ex_data;\n");

  O_CALL (catch, generate);

  fprintf (out, "ex_pop ();\n");
  if (self->finally_clause)
    {
      fprintf (out, "do_finally;\n");
    }
  fprintf (out, "}\n");
}

O_IMPLEMENT(TryStatement, void, generate, (void *_self))
{
  struct TryStatement *self = O_CAST(_self, TryStatement());
  fprintf (out, "try {\n");
  O_CALL (self->try_clause, generate);
  fprintf (out, "ex_pop ();\n");
  if (self->finally_clause)
    {
      fprintf (out, "do_finally;\n");
    }
  fprintf (out, "}\n");

  O_CALL (self->catch_clause, map_args, TryStatement_generate_catch, self);

  if (self->finally_clause)
    {
      fprintf (out, "finally {\n");
      O_CALL (self->finally_clause, generate);
      fprintf (out, "throw (1, NULL, NULL, 0, NULL);\n");
      fprintf (out, "}\n");
      fprintf (out, "end_try_finally;\n");
    }
  else
    {
      fprintf (out, "end_try;\n");
    }
}

O_OBJECT(TryStatement, Statement);
O_OBJECT_METHOD(TryStatement, ctor);
O_OBJECT_METHOD(TryStatement, dtor);
O_OBJECT_METHOD(TryStatement, accept);
O_OBJECT_METHOD(TryStatement, type_check);
O_OBJECT_METHOD(TryStatement, generate);
O_END_OBJECT
