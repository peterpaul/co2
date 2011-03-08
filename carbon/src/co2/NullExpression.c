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
#include "co2/NullExpression.h"
#include "co2/io.h"
#include "co2/PrimitiveType.h"
#include "co2/ArrayType.h"
#include "grammar.h"

#define O_SUPER TokenExpression()

O_IMPLEMENT(NullExpression, void *, ctor, (void *_self, va_list *app))
{
  struct NullExpression * self = O_CAST(_self, NullExpression());
  self = O_SUPER->ctor(self, app);
  return self;
}

O_IMPLEMENT(NullExpression, void *, dtor, (void *_self))
{
  struct NullExpression *self = O_CAST(_self, NullExpression());
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(NullExpression, void, type_check, (void *_self))
{
  struct NullExpression *self = O_CAST(_self, NullExpression());
  struct Token *type_token = O_CALL_CLASS(Token(), new_ctor, _Token_ctor_from_token, self->token, "void", VOID);
  struct Type *void_type = O_CALL_CLASS(PrimitiveType(), new, type_token);
  struct ArrayType *void_ptr_type = O_CALL_CLASS(ArrayType(), new, void_type);
  self->type = O_CALL(void_ptr_type, retain);
}

O_IMPLEMENT(NullExpression, void, generate, (void *_self))
{
  struct NullExpression *self = O_CAST(_self, NullExpression());
  fprintf(out, "NULL");
}

O_OBJECT(NullExpression, TokenExpression);
O_OBJECT_METHOD(NullExpression, ctor);
O_OBJECT_METHOD(NullExpression, dtor);
O_OBJECT_METHOD(NullExpression, type_check);
O_OBJECT_METHOD(NullExpression, generate);
O_END_OBJECT
