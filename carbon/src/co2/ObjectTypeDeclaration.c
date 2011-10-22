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
#include "co2/ObjectTypeDeclaration.h"
#include "co2/FunctionDeclaration.h"
#include "co2/ArgumentDeclaration.h"
#include "co2/VariableDeclaration.h"
#include "co2/ConstructorDeclaration.h"
#include "co2/DestructorDeclaration.h"
#include "co2/InterfaceDeclaration.h"
#include "co2/Type.h"
#include "co2/ObjectType.h"
#include "co2/Token.h"
#include "co2/RefList.h"
#include "co2/FunctionType.h"
#include "co2/io.h"
#include "co2/Statement.h"
#include "co2/Scope.h"
#include "co2/PrimitiveType.h"
#include "grammar.h"

#define O_SUPER Declaration()

O_IMPLEMENT (ObjectTypeDeclaration, void *, ctor,
	     (void *_self, va_list * app))
{
  struct ObjectTypeDeclaration *self =
    O_CAST (_self, ObjectTypeDeclaration ());
  self = O_SUPER->ctor (self, app);
  return self;
}

O_IMPLEMENT (ObjectTypeDeclaration, void *, dtor, (void *_self))
{
  struct ObjectTypeDeclaration *self =
    O_CAST (_self, ObjectTypeDeclaration ());
  O_BRANCH_CALL (self->members, release);
  O_CALL (self->member_scope, delete);
  return O_SUPER->dtor (self);
}

void
ObjectTypeDeclaration_generate_method_arguments (void *_arg)
{
  struct ArgumentDeclaration *arg = O_CAST (_arg, ArgumentDeclaration ());
  fprintf (out, ", ");
  O_CALL (arg, generate);
}

void
ObjectTypeDeclaration_generate_method_argument_names (void *_arg)
{
  struct ArgumentDeclaration *arg = O_CAST (_arg, ArgumentDeclaration ());
  fprintf (out, ", ");
  O_CALL (arg->name, generate);
}

void
ObjectTypeDeclaration_generate_method_registration_2 (void *_method_decl,
						      va_list * app)
{
  struct FunctionDeclaration *method_decl =
    O_CAST (_method_decl, FunctionDeclaration ());
  struct ObjectTypeDeclaration *class_decl =
    O_CAST (va_arg (*app, struct ObjectTypeDeclaration *),
	    ObjectTypeDeclaration ());
  if (!method_decl->body)
    {
      return;
    }
  fprintf (out, "O_OBJECT_METHOD (");
  O_CALL (class_decl->name, generate);
  fprintf (out, ", ");
  O_CALL (method_decl->name, generate);
  fprintf (out, ");\n");
}

O_OBJECT (ObjectTypeDeclaration, Declaration);
O_OBJECT_METHOD (ObjectTypeDeclaration, ctor);
O_OBJECT_METHOD (ObjectTypeDeclaration, dtor);
O_END_OBJECT
