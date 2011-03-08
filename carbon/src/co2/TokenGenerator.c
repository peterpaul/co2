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
#include "co2/TokenGenerator.h"
#include "grammar.h"

#define O_SUPER Singleton()

O_IMPLEMENT (TokenGenerator, void *, ctor, (void *_self, va_list * app))
{
  struct TokenGenerator *self = O_CAST (_self, TokenGenerator ());
  self = O_SUPER->ctor (self, app);
  self->value = 0;
  return self;
}

O_IMPLEMENT (TokenGenerator, void *, dtor, (void *_self))
{
  struct TokenGenerator *self = O_CAST (_self, TokenGenerator ());
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (TokenGenerator, struct Token *, create,
	     (void *_self, struct Token * token))
{
  struct TokenGenerator *self = O_CAST (_self, TokenGenerator ());
  struct String *name =
    O_CALL_CLASS (String (), new, "_%s_%d", token->name->data, self->value);
  struct Token *retval =
    O_CALL_CLASS (Token (), new_ctor, _Token_ctor_from_token, token, name->data, IDENTIFIER);
  self->value++;
  O_CALL (name, delete);
  return retval;
}

O_OBJECT (TokenGenerator, Singleton);
O_OBJECT_METHOD (TokenGenerator, ctor);
O_OBJECT_METHOD (TokenGenerator, dtor);
O_OBJECT_METHOD (TokenGenerator, create);
O_END_OBJECT
