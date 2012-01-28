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
#include "co2/Token.h"
#include "co2/io.h"

#define O_SUPER CompileObject()

O_IMPLEMENT (Token, void *, ctor, (void *_self, va_list * app))
{
  struct Token *self = O_CAST (_self, Token ());
  self = O_SUPER->ctor (self, app);
  self->name = O_CALL_CLASS (String (), new, "%s", va_arg (*app, char *));
  O_CALL (self->name, retain);
  self->type = va_arg (*app, int);
  self->file = O_BRANCH_GET_ARG (File); // don't retain, this is a weak reference
  self->line = va_arg (*app, int);
  return self;
}

O_IMPLEMENT (Token, void *, ctor_from_token, (void *_self, va_list * app))
{
  struct Token *self = O_CAST (_self, Token ());
  self = O_SUPER->ctor (self, app);
  struct Token *base = O_CAST (va_arg (*app, struct Token *), Token ());
  self->name = O_CALL_CLASS (String (), new, "%s", va_arg (*app, char *));
  O_CALL (self->name, retain);
  self->type = va_arg (*app, int);
  self->file = base->file; // don't retain, this is a weak reference
  self->line = base->line;
  return self;
}

O_IMPLEMENT (Token, void *, dtor, (void *_self))
{
  struct Token *self = O_CAST (_self, Token ());
  O_CALL (self->name, release);
  return O_SUPER->dtor (self);  
}

O_IMPLEMENT (Token, void, generate, (void *_self))
{
  struct Token *self = O_CAST (_self, Token ());
  fprintf (out, "%s", self->name->data);
}

O_OBJECT (Token, CompileObject);
O_OBJECT_METHOD (Token, ctor);
O_OBJECT_METHOD (Token, ctor_from_token);
O_OBJECT_METHOD (Token, dtor);
O_OBJECT_METHOD (Token, generate);
O_OBJECT_METHOD (CompileObject, accept);
O_END_OBJECT
