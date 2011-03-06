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
#include "co2/SizeExpression.h"
#include "co2/io.h"
#include "grammar.h"
#include "co2/PrimitiveType.h"
#include "co2/Token.h"

#define O_SUPER Expression()

O_IMPLEMENT(SizeExpression, void *, ctor, (void *_self, va_list *app))
{
  struct SizeExpression * self = O_CAST(_self, SizeExpression());
  self = O_SUPER->ctor(self, app);
  self->param = O_RETAIN_ARG (Type);
  return self;
}

O_IMPLEMENT(SizeExpression, void *, dtor, (void *_self))
{
  struct SizeExpression *self = O_CAST(_self, SizeExpression());
  O_CALL(self->param, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT (SizeExpression, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct SizeExpression *self = O_CAST (_self, SizeExpression ());
  O_CALL (self->param, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT(SizeExpression, void, type_check, (void *_self))
{
  struct SizeExpression *self = O_CAST(_self, SizeExpression());
  O_CALL(self->param, type_check);
  struct Token * type_token = O_CALL(self->param, get_token);
  struct Token * int_token = O_CALL_CLASS(Token (), new_ctor, _Token_ctor_from_token, type_token, "int", INT);
  self->type = O_CALL_CLASS(PrimitiveType(), new, int_token);
  O_CALL(self->type, retain);
}

O_IMPLEMENT(SizeExpression, void, generate, (void *_self))
{
  struct SizeExpression *self = O_CAST(_self, SizeExpression());
  fprintf(out, "sizeof (");
  O_CALL(self->param, generate);
  fprintf(out, ")");
}

O_OBJECT(SizeExpression, Expression);
O_OBJECT_METHOD(SizeExpression, ctor);
O_OBJECT_METHOD(SizeExpression, dtor);
O_OBJECT_METHOD(SizeExpression, accept);
O_OBJECT_METHOD(SizeExpression, generate);
O_OBJECT_METHOD(SizeExpression, type_check);
O_END_OBJECT
