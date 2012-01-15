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
#include "co2/GenerateSourceVisitor.h"
#include "co2/CompileObject.h"
#include "co2/ArgumentDeclaration.h"
#include "co2/ClassDeclaration.h"
#include "co2/VariableDeclaration.h"
#include "co2/FunctionDeclaration.h"
#include "co2/InterfaceDeclaration.h"
#include "co2/InterfaceMethodDefinition.h"
#include "co2/ConstructorDeclaration.h"
#include "co2/DestructorDeclaration.h"
#include "co2/StructDeclaration.h"
#include "co2/TypeDeclaration.h"
#include "co2/FunctionType.h"
#include "co2/Statement.h"
#include "co2/PrimitiveType.h"
#include "grammar.h"
#include "co2/Expression.h"
#include "co2/FunctionType.h"
#include "co2/ObjectType.h"
#include "co2/Type.h"
#include "co2/io.h"

static void
ClassDeclaration_generate_constructor_arguments (void *_arg)
{
  struct ArgumentDeclaration *arg = O_CAST (_arg, ArgumentDeclaration ());
  if (arg->name->type == VA_ARG)
    {
      fprintf (out, "va_list ap;\n");
      fprintf (out, "va_copy (ap, *app);\n");
    }
  else
    {
      O_CALL (arg, generate);
      if (o_is_a (arg->type, ObjectType ()))
	{
	  struct ObjectType *type = (struct ObjectType *) arg->type;
	  if (o_is_of (type->decl, ClassDeclaration ()))
	    {
	      fprintf (out, " = O_BRANCH_CAST (va_arg(*app, ");
	      O_CALL (type, generate);
	      fprintf (out, "), ");
	      O_CALL (type->token, generate);
	      fprintf (out, "());\n");
	      return;
	    }
	}
      fprintf (out, " = va_arg (*app, ");
      O_CALL (arg->type, generate);
      fprintf (out, ");\n");
    }
}

/**
 * Iterate over all interfaces implemented by a class.
 */
static void
ClassDeclaration_generate_method_implementation_2 (void *_interface_name,
						   va_list * app)
{
  struct Token *interface_name = O_CAST (_interface_name, Token ());
  struct ClassDeclaration *class_decl = O_GET_ARG (ClassDeclaration);

  struct Declaration *_decl =
    O_CALL_IF (IScope, global_scope, lookup_in_this_scope, interface_name);
  struct InterfaceDeclaration *interface_decl =
    O_CAST (_decl, InterfaceDeclaration ());

  /**
   * Iterate over all methods of a class.
   */
  void generate_method_bindings_filter_1 (void *_decl)
  {
    if (o_is_of (_decl, FunctionDeclaration ()))
      {
	struct FunctionDeclaration * method_decl = O_CAST (_decl, FunctionDeclaration ());

	/**
	 * Iterate over all implemented_methods to find the interface.
	 */
	void generate_method_bindings_filter_2 (void *_if)
	{
	  struct InterfaceMethodDefinition * imd = o_cast (_if, InterfaceMethodDefinition ());

	  if (interface_implements (imd->interface_decl, interface_decl)
	      && O_CALL_IF (IScope, interface_decl->member_scope, exists, imd->method_decl->name))
	    {
	      fprintf (out, "O_OBJECT_IF_METHOD_BINDING (");
	      O_CALL (class_decl->name, generate);
	      fprintf (out, ", ");
	      O_CALL (imd->method_name, generate);
	      fprintf (out, ", ");
	      O_CALL (method_decl->name, generate);
	      fprintf (out, ");\n");
	    }
	}
	
	O_CALL (method_decl->implemented_methods, map, generate_method_bindings_filter_2);
      }
  }

  fprintf (out, "O_OBJECT_IF (");
  O_CALL (interface_name, generate);
  fprintf (out, ");\n");

  O_CALL (class_decl->members, map, generate_method_bindings_filter_1);

  fprintf (out, "O_OBJECT_IF_END\n");
  
  O_BRANCH_CALL (interface_decl->interfaces, map_args, ClassDeclaration_generate_method_implementation_2, class_decl);
}

static void ClassDeclaration_generate_interface_method_binding (void *_interface_name, va_list *app)
{
  struct Token *interface_name = O_CAST (_interface_name, Token ());
  struct ClassDeclaration *class_decl = O_GET_ARG (ClassDeclaration);

  struct Declaration *_decl =
    O_CALL_IF (IScope, global_scope, lookup_in_this_scope, interface_name);
  struct InterfaceDeclaration *interface_decl =
    O_CAST (_decl, InterfaceDeclaration ());

  void generate_interface_method_binding_impl (void *_decl)
  {
    struct FunctionDeclaration * if_method_decl = O_CAST (_decl, FunctionDeclaration ());

    struct FunctionDeclaration * method_decl = NULL;
    if (O_CALL_IF (IScope, class_decl->member_scope, exists, if_method_decl->name))
      {
	method_decl = O_BRANCH_CAST (O_CALL_IF (IScope, class_decl->member_scope, lookup, if_method_decl->name), FunctionDeclaration ());
      }
    if (if_method_decl->body && (method_decl && !method_decl->body))
      {
	if (!method_decl->binding_generated)
	  {
	    struct FunctionType *method_type =
	      o_cast (if_method_decl->type, FunctionType ());
	    fprintf (out, "O_IMPLEMENT_IF_BINDING (");
	    O_CALL (class_decl->name, generate);
	    fprintf (out, ", ");
	    O_CALL (method_type->return_type, generate);
	    fprintf (out, ", ");
	    O_CALL (if_method_decl->name, generate);
	    fprintf (out, ", (void *_self");
	    O_CALL (if_method_decl->formal_arguments, map,
		    ObjectTypeDeclaration_generate_method_arguments);
	    fprintf (out, "), (_self");
	    O_CALL (if_method_decl->formal_arguments, map,
		    ObjectTypeDeclaration_generate_method_argument_names);
	    fprintf (out, "));\n");
	    method_decl->binding_generated = true;
	  }
      }
  }

  O_CALL (interface_decl->members, map, generate_interface_method_binding_impl);

  O_BRANCH_CALL (interface_decl->interfaces, map_args, ClassDeclaration_generate_interface_method_binding, class_decl);
}

#define O_SUPER BaseCompileObjectVisitor()

O_IMPLEMENT(GenerateSourceVisitor, void *, ctor, (void *_self, va_list *app))
{
  struct GenerateSourceVisitor * self = O_CAST(_self, GenerateSourceVisitor());
  self = O_SUPER->ctor(self, app);
  self->fp = va_arg (*app, FILE *);
  current_context = O_CALL_CLASS (Context (), new);
  O_BRANCH_CALL (current_context, retain);
  return self;
}

O_IMPLEMENT(GenerateSourceVisitor, void *, dtor, (void *_self))
{
  struct GenerateSourceVisitor *self = O_CAST(_self, GenerateSourceVisitor());
  O_BRANCH_CALL (current_context, release);
  current_context = NULL;
  return O_SUPER->dtor(self);
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitClassDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct ClassDeclaration *self = O_CAST(_object, ClassDeclaration ());

  /* filter the members */
  struct RefList *methods =
    O_CALL (self->members, filter_args, type_filter, FunctionDeclaration ());
  O_CALL (methods, retain);
  struct RefList *constructors =
    O_CALL (self->members, filter_args, type_filter,
	    ConstructorDeclaration ());
  O_CALL (constructors, retain);
  struct RefList *destructors =
    O_CALL (self->members, filter_args, type_filter,
	    DestructorDeclaration ());
  O_CALL (destructors, retain);

  O_CALL (self->interfaces, map_args, ClassDeclaration_generate_interface_method_binding, self);
  fprintf (out, "\n");

  fprintf (out, "#define O_SUPER ");
  generate_superclass (self);
  fprintf (out, " ()\n\n");

  fprintf (out, "O_OBJECT (");
  O_CALL (self->name, generate);
  fprintf (out, ", ");
  generate_superclass (self);
  fprintf (out, ");\n");

  void generate_constructor_registration_2 (void *_constructor_decl)
  {
    struct ConstructorDeclaration *constructor_decl =
      O_CAST (_constructor_decl, ConstructorDeclaration ());
    fprintf (out, "O_OBJECT_METHOD (");
    O_CALL (self->name, generate);
    fprintf (out, ", ");
    if (strcmp (constructor_decl->name->name->data, "ctor") != 0)
      {
	fprintf (out, "ctor_");
      }
    O_CALL (constructor_decl->name, generate);
    fprintf (out, ");\n");
  }

  O_CALL (constructors, map,
	  generate_constructor_registration_2);

  void generate_destructor_registration_2 (void *_destructor_decl)
  {
    struct DestructorDeclaration *destructor_decl =
      O_CAST (_destructor_decl, DestructorDeclaration ());
    fprintf (out, "O_OBJECT_METHOD (");
    O_CALL (self->name, generate);
    fprintf (out, ", dtor);\n");
  }

  O_CALL (destructors, map,
	  generate_destructor_registration_2);
  O_CALL (methods, map_args,
	  ObjectTypeDeclaration_generate_method_registration_2, self);

  O_BRANCH_CALL (self->interfaces, map_args,
		 ClassDeclaration_generate_method_implementation_2, self);

  fprintf (out, "O_END_OBJECT\n\n");

  fprintf (out, "#undef O_SUPER\n\n");

  O_CALL (methods, release);
  O_CALL (constructors, release);
  O_CALL (destructors, release);
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitConstructorDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct ConstructorDeclaration *self =
    O_CAST (_object, ConstructorDeclaration ());
  struct ClassDeclaration *class_decl = O_CALL (current_context, find, ClassDeclaration ());
  // don't generate for included classes
  if (class_decl->include_file)
    {
      return;
    }
  fprintf (out, "O_IMPLEMENT (");
  O_CALL (class_decl->name, generate);
  fprintf (out, ", void *");
  fprintf (out, ", ");
  if (strcmp (self->name->name->data, "ctor") != 0)
    {
      fprintf (out, "ctor_");
    }
  O_CALL (self->name, generate);
  fprintf (out, ", (void *_self, va_list *app))\n");
  fprintf (out, "{\n");
  fprintf (out, "struct ");
  O_CALL (class_decl->name, generate);
  fprintf (out, "* self = O_CAST (_self, ");
  O_CALL (class_decl->name, generate);
  fprintf (out, " ());\n");
  O_CALL (self->formal_arguments, map,
	  ClassDeclaration_generate_constructor_arguments);

  O_CALL (self->body, generate);
  if (self->formal_arguments->length > 0)
    {
      struct ArgumentDeclaration * last_arg = O_CALL (self->formal_arguments, get, self->formal_arguments->length -1);
      if (last_arg->name->type == VA_ARG)
	{
	  fprintf (out, "va_end (ap);\n");
	}
    }
  fprintf (out, "return self;\n");
  fprintf (out, "}\n\n");
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceVisitor *visitor = O_CAST (_self, GenerateSourceVisitor ());
  struct Declaration *self = O_CAST (_object, Declaration ());
  if (self->include_file)
    {
      // don't generate if external definition
      return;
    }
  else
    {
      O_SUPER->visitDeclaration(visitor, self);
    }
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitDestructorDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateSourceVisitor *visitor = O_CAST (_self, GenerateSourceVisitor ());
  struct DestructorDeclaration *self = O_CAST (_object, DestructorDeclaration ());

  struct ClassDeclaration *class_decl = O_CALL (current_context, find, ClassDeclaration ());
  // don't generate for included classes
  if (class_decl->include_file)
    {
      return;
    }

  fprintf (out, "O_IMPLEMENT (");
  O_CALL (class_decl->name, generate);
  fprintf (out, ", void *");
  fprintf (out, ", dtor, (void *_self))\n");
  fprintf (out, "{\n");
  fprintf (out, "struct ");
  O_CALL (class_decl->name, generate);
  fprintf (out, "* self = O_CAST (_self, ");
  O_CALL (class_decl->name, generate);
  fprintf (out, " ());\n");
  O_CALL (self->body, generate);
  fprintf (out, "return ");
  if (class_decl->superclass)
    {
      O_CALL (class_decl->superclass, generate);
    }
  else
    {
      fprintf (out, "Object");
    }
  fprintf (out, " ()->dtor (self);\n");
  fprintf (out, "}\n\n");
}

void GenerateSourceVisitor_visitFunctionDeclaration_generateImplementation(struct FunctionDeclaration * self, void * _parent_decl)
{
  struct Declaration *parent_decl = O_CAST (_parent_decl, Declaration ());
  struct FunctionType *method_type =
    o_cast (self->type, FunctionType ());
  if (self->implemented_methods->length > 0
      && !O_CALL (current_context, find, InterfaceDeclaration ()))
    {
      fprintf (out, "O_IMPLEMENT_IF (");
    }
  else
    {
      fprintf (out, "O_IMPLEMENT (");
    }
  O_CALL (parent_decl->name, generate);
  fprintf (out, ", ");
  O_CALL (method_type->return_type, generate);
  fprintf (out, ", ");
  O_CALL (self->name, generate);
  fprintf (out, ", (void *_self");
  O_CALL (self->formal_arguments, map,
	  ObjectTypeDeclaration_generate_method_arguments);
  if (self->implemented_methods->length > 0
      && !O_CALL (current_context, find, InterfaceDeclaration ()))
    {
      fprintf (out, "), (_self");
      O_CALL (self->formal_arguments, map,
	      ObjectTypeDeclaration_generate_method_argument_names);
    }
  fprintf (out, "))\n");
  fprintf (out, "{\n");

  if (o_is_of (parent_decl, InterfaceDeclaration ()))
    {
      fprintf (out, "struct Object * self = O_CAST (_self, Object ());\n");
      fprintf (out, "O_IS_IMPLEMENTATION (self, ");
      O_CALL (parent_decl->name, generate);
      fprintf (out, " ());\n");
    }
  else
    {
      fprintf (out, "struct ");
      O_CALL (parent_decl->name, generate);
      fprintf (out, "* self = O_CAST (_self, ");
      O_CALL (parent_decl->name, generate);
      fprintf (out, " ());\n");
    }
  if (method_type->has_var_args)
    {
      fprintf (out, "va_list ap;\n");
      fprintf (out, "va_start (ap, ");
      if (self->formal_arguments->length == 1)
	{
	  fprintf (out, "_self");
	}
      else
	{
	  struct ArgumentDeclaration *arg_decl =
	    O_CALL (self->formal_arguments, get,
		    self->formal_arguments->length - 2);
	  O_CALL (arg_decl->name, generate);
	}
      fprintf (out, ");\n");
    }
  
  O_CALL (self->body, generate);
  
  if (method_type->has_var_args && 
      (o_is_of (method_type->return_type, PrimitiveType ())
       && ((struct PrimitiveType *) method_type->return_type)->token->type == VOID))
    {
      fprintf (out, "va_end (ap);\n");
    }
      
  fprintf (out, "}\n\n");
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitFunctionDeclaration, (void *_self, void *_object), (_self, _object))
{
  // Don't generate for nested functions
  if (O_CALL (current_context, find_nth_instance, FunctionDeclaration (), 2))
    {
      return;
    }

  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct FunctionDeclaration *self = O_CAST(_object, FunctionDeclaration ());

  struct ClassDeclaration *class_decl = O_CALL (current_context, find, ClassDeclaration ());
  struct InterfaceDeclaration *interface_decl = O_CALL (current_context, find, InterfaceDeclaration ());
  if (class_decl)
    {
      // don't generate for included classes
      if (class_decl->include_file)
	{
	  return;
	}
      // don't generate for definitions
      if (!self->body)
	{
	  return;
	}
      GenerateSourceVisitor_visitFunctionDeclaration_generateImplementation (self, class_decl);
    }
  else if (interface_decl)
    {
      // don't generate for included classes
      if (interface_decl->include_file)
	{
	  return;
	}
      struct FunctionType *method_type =
	o_cast (self->type, FunctionType ());
      fprintf (out, "O_METHOD_IF (");
      O_CALL (interface_decl->name, generate);
      fprintf (out, ", ");
      O_CALL (method_type->return_type, generate);
      fprintf (out, ", ");
      O_CALL (self->name, generate);
      fprintf (out, ", (void *_self");
      O_CALL (self->formal_arguments, map,
	      ObjectTypeDeclaration_generate_method_arguments);
      fprintf (out, "), (_self");
      O_CALL (self->formal_arguments, map,
	      ObjectTypeDeclaration_generate_method_argument_names);
      fprintf (out, "));\n");
      if (self->body)
	{
	  GenerateSourceVisitor_visitFunctionDeclaration_generateImplementation (self, interface_decl);
	}
    }
  else
    {
      if (O_CALL_IF (IScope, self->scope, get_type) != GLOBAL_SCOPE)
	{
	  return;
	}
      FunctionDeclaration_generateFunction (self);
    }
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitInterfaceDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct InterfaceDeclaration *self = O_CAST (_object, InterfaceDeclaration ());

  fprintf (out, "#define O_SUPER Interface ()\n\n");

  fprintf (out, "O_OBJECT (");
  O_CALL (self->name, generate);
  fprintf (out, ", Interface);\n");
  fprintf (out, "O_END_OBJECT\n\n");

  fprintf (out, "#undef O_SUPER\n\n");
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitVariableDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct VariableDeclaration *self = O_CAST(_object, VariableDeclaration());

  if (self->scope && O_CALL_IF (IScope, self->scope, get_type) != GLOBAL_SCOPE)
    {
      // only generate global declarations
      return;
    }

  if (o_is_of (self->type, FunctionType ()))
    {
      struct FunctionType * function_type = O_CAST (self->type, FunctionType ());
      O_CALL (function_type, generate_named, self->name);
    }
  else
    {
      O_CALL (self->type, generate);
      fprintf (out, " ");
      O_CALL (self->name, generate);
    }
  if (self->expr)
    {
      fprintf (out, " = ");
      O_CALL (self->expr, generate);
    }
  fprintf (out, ";\n");
}

O_OBJECT (GenerateSourceVisitor, BaseCompileObjectVisitor);
O_OBJECT_METHOD (GenerateSourceVisitor, ctor);
O_OBJECT_METHOD (GenerateSourceVisitor, dtor);
O_OBJECT_METHOD (GenerateSourceVisitor, visitClassDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitConstructorDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitDestructorDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitFunctionDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitInterfaceDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitVariableDeclaration);
O_OBJECT_IF(CompileObjectVisitor);
O_OBJECT_IF_METHOD (GenerateSourceVisitor, visitClassDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceVisitor, visitConstructorDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceVisitor, visitDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceVisitor, visitDestructorDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceVisitor, visitFunctionDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceVisitor, visitInterfaceDeclaration);
O_OBJECT_IF_METHOD (GenerateSourceVisitor, visitVariableDeclaration);
O_OBJECT_IF_END
O_END_OBJECT
