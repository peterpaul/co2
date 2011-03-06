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
#include "co2/Path.h"
#include "co2/RefList.h"
#include "co2/Token.h"

#define O_SUPER CompileObject()

O_IMPLEMENT (Path, void *, ctor, (void *_self, va_list * app))
{
  struct Path *self = O_CAST (_self, Path ());
  self = O_SUPER->ctor (self, app);
  self->path_name = O_RETAIN_ARG (RefList);
  return self;
}

O_IMPLEMENT (Path, void *, dtor, (void *_self))
{
  struct Path *self = O_CAST (_self, Path ());
  O_CALL (self->path_name, release);
  return O_SUPER->dtor (self);
}

void
Path_convert_to_system_path (void *_token, va_list * app)
{
  struct Token *token = O_CAST (_token, Token ());
  struct String *result = O_CAST (va_arg (*app, struct String *), String ());
  O_CALL (result, append_str, "/");
  O_CALL (result, append, token->name);
}

O_IMPLEMENT (Path, struct String *, to_system_path, (void *_self))
{
  struct Path *self = O_CAST (_self, Path ());
  struct String *result = O_CALL_CLASS (String (), new, "");
  O_CALL (self->path_name, map_args, Path_convert_to_system_path, result);
  return result;
}

O_OBJECT (Path, CompileObject);
O_OBJECT_METHOD (Path, ctor);
O_OBJECT_METHOD (Path, dtor);
O_OBJECT_METHOD (Path, to_system_path);
O_END_OBJECT
