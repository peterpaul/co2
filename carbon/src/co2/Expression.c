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
#include "co2/Expression.h"
#include "co2/Type.h"
#include "co2/io.h"

#define O_SUPER CompileObject()

O_IMPLEMENT (Expression, void *, ctor, (void *_self, va_list * app))
{
  struct Expression *self = O_CAST (_self, Expression ());
  self = O_SUPER->ctor (self, app);
  return self;
}

O_IMPLEMENT (Expression, void *, dtor, (void *_self))
{
  struct Expression *self = O_CAST (_self, Expression ());
  O_BRANCH_CALL (self->type, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (Expression, void, generate, (void *_self))
{
  struct Expression *self = O_CAST (_self, Expression ());
  O_CALL (self, generate_left, true);
}

O_OBJECT (Expression, CompileObject);
O_OBJECT_METHOD (Expression, ctor);
O_OBJECT_METHOD (Expression, dtor);
O_OBJECT_METHOD (Expression, generate);
O_END_OBJECT 

/**
 * Generates an argument expression.
 */
void
Expression_generate_actual_argument (void *_arg, va_list * app)
{
  struct Expression *arg = O_CAST (_arg, Expression ());
  bool *is_first_arg = va_arg (*app, bool *);
  if (!*is_first_arg)
    {
      fprintf (out, ", ");
    }
  else
    {
      *is_first_arg = false;
    }
  O_CALL (arg, generate);
}
