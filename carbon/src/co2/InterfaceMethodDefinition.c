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
#include "co2/InterfaceMethodDefinition.h"

#define O_SUPER CompileObject()

O_IMPLEMENT(InterfaceMethodDefinition, void *, ctor, (void *_self, va_list *app))
{
  struct InterfaceMethodDefinition * self = O_CAST(_self, InterfaceMethodDefinition());
  self = O_SUPER->ctor(self, app);
  self->interface_name = O_RETAIN_ARG (Token);
  self->method_name = O_BRANCH_RETAIN_ARG (Token);
  return self;
}

O_IMPLEMENT(InterfaceMethodDefinition, void *, dtor, (void *_self))
{
  struct InterfaceMethodDefinition *self = O_CAST(_self, InterfaceMethodDefinition());
  O_CALL (self->interface_name, release);
  O_BRANCH_CALL (self->method_name, release);
  return O_SUPER->dtor(self);
}

O_OBJECT(InterfaceMethodDefinition, CompileObject);
O_OBJECT_METHOD(InterfaceMethodDefinition, ctor);
O_OBJECT_METHOD(InterfaceMethodDefinition, dtor);
O_END_OBJECT
