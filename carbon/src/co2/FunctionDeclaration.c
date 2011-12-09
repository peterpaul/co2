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
#include "co2/FunctionDeclaration.h"
#include "co2/Type.h"
#include "co2/RefList.h"
#include "co2/Statement.h"
#include "co2/io.h"
#include "co2/FunctionType.h"
#include "co2/ArgumentDeclaration.h"
#include "co2/ClassDeclaration.h"
#include "co2/InterfaceDeclaration.h"
#include "co2/InterfaceMethodDefinition.h"
#include "grammar.h"
#include "co2/PrimitiveType.h"

#define O_SUPER Declaration()

O_IMPLEMENT (FunctionDeclaration, void *, ctor, (void *_self, va_list * app))
{
  struct FunctionDeclaration *self = O_CAST (_self, FunctionDeclaration ());
  self = O_SUPER->ctor (self, app);
  self->type = O_RETAIN_ARG (FunctionType);
  self->formal_arguments = O_RETAIN_ARG (RefList);
  self->body = O_BRANCH_RETAIN_ARG (Statement);
  self->binding_generated = false;
  return self;
}

O_IMPLEMENT (FunctionDeclaration, void *, dtor, (void *_self))
{
  struct FunctionDeclaration *self = O_CAST (_self, FunctionDeclaration ());
  O_CALL (self->formal_arguments, release);
  O_BRANCH_CALL (self->implemented_methods, release);
  O_BRANCH_CALL (self->body, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (FunctionDeclaration, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct FunctionDeclaration *self = O_CAST (_self, FunctionDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->type, accept, visitor);
  O_CALL (self->formal_arguments, map_args, accept, visitor);
  O_BRANCH_CALL (self->body, accept, visitor);
  O_CALL (visitor, visit, self);
  O_BRANCH_CALL (current_context, remove_last);
}

void
FunctionDeclaration_generate_formal_arg (void *_decl, va_list * ap)
{
  struct Declaration *decl = O_CAST (_decl, Declaration ());
  bool *first_formal_arg = va_arg (*ap, bool *);
  if (*first_formal_arg == false)
    {
      fprintf (out, ", ");
    }
  *first_formal_arg = false;
  O_CALL (decl, generate);
}

void FunctionDeclaration_generateFunction (struct FunctionDeclaration *self)
{
  // don't generate for definitions
  if (!self->body)
    {
      return;
    }
  bool first_formal_arg = true;
  struct FunctionType *function_type = o_cast (self->type, FunctionType ());
  O_CALL (function_type->return_type, generate);
  fprintf (out, " ");
  O_CALL (self->name, generate);
  fprintf (out, " (");
  O_CALL (self->formal_arguments, map_args,
	  FunctionDeclaration_generate_formal_arg, &first_formal_arg);
  fprintf (out, ")\n");
  fprintf (out, "{\n");
      
  if (function_type->has_var_args)
    {
      fprintf (out, "va_list ap;\n");
      fprintf (out, "va_start (ap, ");
      struct ArgumentDeclaration *arg_decl =
	O_CALL (self->formal_arguments, get,
		self->formal_arguments->length - 2);
      O_CALL (arg_decl->name, generate);
      fprintf (out, ");\n");
    }
  O_BRANCH_CALL (self->body, generate);
      
  if (function_type->has_var_args && 
      (o_is_of (function_type->return_type, PrimitiveType ())
       && ((struct PrimitiveType *) function_type->return_type)->token->type == VOID))
    {
      fprintf (out, "va_end (ap);\n");
    }
      
  fprintf (out, "}\n\n");
}

static void
FunctionDeclaration_find_in_interface (void *_interface_name, va_list * app)
{
  struct Token *interface_name = O_CAST (_interface_name, Token ());
  struct FunctionDeclaration *function_decl =
    O_CAST (va_arg (*app, struct FunctionDeclaration *),
	    FunctionDeclaration ());
  if (O_CALL_IF (IScope, global_scope, exists_in_this_scope, interface_name))
    {
      struct Declaration *_decl =
	O_CALL_IF (IScope, global_scope, lookup_in_this_scope, interface_name);
      if (o_is_of (_decl, InterfaceDeclaration ()))
	{
	  struct InterfaceDeclaration * interface_decl =
	    O_CAST (_decl, InterfaceDeclaration ());
	  if (O_CALL_IF (IScope, interface_decl->member_scope, exists,
		      function_decl->name))
	    {
	      if (function_decl->body)
		{
		  struct InterfaceMethodDefinition * imd = O_CALL_CLASS (InterfaceMethodDefinition (), new, interface_decl->name, function_decl->name);
		  imd->interface_decl = O_CALL (interface_decl, retain);
		  imd->method_decl = (struct FunctionDeclaration *) O_CALL_IF (IScope, interface_decl->member_scope, lookup, function_decl->name);
		  O_CALL (imd->method_decl, retain);
		  O_CALL (function_decl->implemented_methods, append, imd);
		}
	    }
	  O_BRANCH_CALL (interface_decl->interfaces, map_args, FunctionDeclaration_find_in_interface, function_decl);
	}
      else
	{
	  error (function_decl->name, "%s is not an interface\n",
		 _decl->name->name->data);
	}
    }
}

static void
FunctionDeclaration_find_in_interface_ (void *_interface_name, ...)
{
  va_list ap;
  va_start (ap, _interface_name);
  FunctionDeclaration_find_in_interface (_interface_name, &ap);
  va_end (ap);
}

static void FunctionDeclaration_type_check_implemented_methods (void *_imd, va_list *app)
{
  struct InterfaceMethodDefinition * imd = O_CAST (_imd, InterfaceMethodDefinition ());
  struct FunctionDeclaration * fun_decl = O_GET_ARG (FunctionDeclaration);

  if (!imd->interface_decl)
    {
      struct Declaration *_interface_decl = O_CALL_IF (IScope, global_scope, lookup, imd->interface_name);
      if (!o_is_of(_interface_decl, InterfaceDeclaration ()))
	{
	  error (imd->interface_name, "Not an interface: '%s'\n", imd->interface_name->name->data);
	  return;
	}
      imd->interface_decl = O_CAST (O_CALL (_interface_decl, retain), InterfaceDeclaration ());
    }
  if (!imd->method_decl)
    {
      if (imd->method_name == NULL)
	{
	  imd->method_name = O_CALL (fun_decl->name, retain);
	}
      struct Declaration *_method_decl = O_CALL_IF (IScope, imd->interface_decl->member_scope, lookup, imd->method_name);
      if (!o_is_of(_method_decl, FunctionDeclaration ()))
	{
	  error (imd->method_name, "Not a function: '%s'\n", imd->method_name->name->data);
	  return;
	}
      imd->method_decl = O_CAST (O_CALL (_method_decl, retain), FunctionDeclaration ());
    }
}

O_IMPLEMENT (FunctionDeclaration, void, type_check, (void *_self))
{
  struct FunctionDeclaration *self = O_CAST (_self, FunctionDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->type, type_check);
  O_CALL (self->formal_arguments, map, CompileObject_type_check);
  O_BRANCH_CALL (self->implemented_methods, map_args, FunctionDeclaration_type_check_implemented_methods, self);
  O_BRANCH_CALL (self->body, type_check);

  struct FunctionType *function_type = o_cast (self->type, FunctionType ());
  struct ClassDeclaration *class_decl =
    O_BRANCH_CALL (current_context, find, ClassDeclaration ());
  struct InterfaceDeclaration *interface_decl =
    O_BRANCH_CALL (current_context, find, InterfaceDeclaration ());
  if (function_type->has_var_args && class_decl == NULL && self->formal_arguments->length <= 1)
    {
      error (self->name,
	     "variable argument list only supported on functions with at least one fixed argument.\n");
    }
  if (class_decl)
    {
      O_BRANCH_CALL (class_decl->interfaces, map_args,
	      FunctionDeclaration_find_in_interface, self);
    }
  if (interface_decl)
    {
      FunctionDeclaration_find_in_interface_ (interface_decl->name, self);
    }
  O_BRANCH_CALL (current_context, remove_last);
}

O_IMPLEMENT (FunctionDeclaration, void, generate, (void *_self))
{
  struct FunctionDeclaration *self = O_CAST (_self, FunctionDeclaration ());
  FunctionDeclaration_generateFunction (self);
}

O_OBJECT (FunctionDeclaration, Declaration);
O_OBJECT_METHOD (FunctionDeclaration, ctor);
O_OBJECT_METHOD (FunctionDeclaration, dtor);
O_OBJECT_METHOD (FunctionDeclaration, accept);
O_OBJECT_METHOD (FunctionDeclaration, type_check);
O_OBJECT_METHOD (FunctionDeclaration, generate);
O_END_OBJECT
