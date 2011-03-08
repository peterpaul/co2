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
#include "co2/PrimitiveType.h"
#include "co2/Token.h"
#include "co2/ObjectType.h"
#include "grammar.h"
#include "co2/io.h"

#define O_SUPER Type()

O_IMPLEMENT (PrimitiveType, void *, ctor, (void *_self, va_list * app))
{
  struct PrimitiveType *self = O_CAST (_self, PrimitiveType ());
  self = O_SUPER->ctor (self, app);
  self->token = O_RETAIN_ARG (Token);
  return self;
}

O_IMPLEMENT (PrimitiveType, void *, dtor, (void *_self))
{
  struct PrimitiveType *self = O_CAST (_self, PrimitiveType ());
  O_CALL (self->token, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (PrimitiveType, void, generate, (void *_self))
{
  struct PrimitiveType *self = O_CAST (_self, PrimitiveType ());
  O_CALL (self->token, generate);
}

O_IMPLEMENT (PrimitiveType, struct Token *, get_token, (void *_self))
{
  struct PrimitiveType *self = O_CAST (_self, PrimitiveType ());
  return self->token;
}

O_IMPLEMENT (PrimitiveType, struct String *, to_string, (void *_self))
{
  struct PrimitiveType *self = O_CAST (_self, PrimitiveType ());
  return O_CALL_CLASS (String (), new, "%s", self->token->name->data);;
}

O_IMPLEMENT (PrimitiveType, bool, is_compatible, (void *_self, void *_other))
{
  struct PrimitiveType *self = O_CAST (_self, PrimitiveType ());
  if (O_SUPER->is_compatible (self, _other))
    {
      if (o_is_of(_other, ObjectType ()))
	{
	  struct ObjectType *other = O_CAST (_other, ObjectType ());
	  return O_CALL (other, is_compatible, self);
	}
      struct PrimitiveType *other = O_CAST (_other, PrimitiveType ());
      struct Token *name_self = O_CALL (self, get_token);
      struct Token *name_other = O_CALL (other, get_token);
      if ((name_self->type == INT || name_self->type == UNSIGNED) &&
	  (name_other->type == INT || name_other->type == UNSIGNED))
	{
	  if (name_self->type != name_other->type)
	    {
	      warning (name_self, "possible data loss while converting %s to %s\n", name_other->name->data, name_self->name->data);
	    }
	  return true;
	}
      else if ((name_self->type == FLOAT || name_self->type == DOUBLE) &&
	  (name_other->type == FLOAT || name_other->type == DOUBLE))
	{
	  if (name_self->type != name_other->type)
	    {
	      warning (name_self, "possible data loss while converting %s to %s\n", name_other->name->data, name_self->name->data);
	    }
	  return true;
	}
      else
	{
	  return name_self->type == name_other->type;
	}
    }
  return false;
}

O_OBJECT (PrimitiveType, Type);
O_OBJECT_METHOD (PrimitiveType, ctor);
O_OBJECT_METHOD (PrimitiveType, dtor);
O_OBJECT_METHOD (PrimitiveType, generate);
O_OBJECT_METHOD (PrimitiveType, get_token);
O_OBJECT_METHOD (PrimitiveType, to_string);
O_OBJECT_METHOD (PrimitiveType, is_compatible);
O_END_OBJECT
