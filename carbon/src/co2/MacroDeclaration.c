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
#include "co2/MacroDeclaration.h"

#define O_SUPER Declaration()

O_IMPLEMENT (MacroDeclaration, void *, ctor, (void *_self, va_list * app))
{
  struct MacroDeclaration *self = O_CAST (_self, MacroDeclaration ());
  self = O_SUPER->ctor (self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT (MacroDeclaration, void *, dtor, (void *_self))
{
  struct MacroDeclaration *self = O_CAST (_self, MacroDeclaration ());
  /* TODO cleanup */
  return O_SUPER->dtor (self);
}

O_OBJECT (MacroDeclaration, Declaration);
O_OBJECT_METHOD (MacroDeclaration, ctor);
O_OBJECT_METHOD (MacroDeclaration, dtor);
O_END_OBJECT
