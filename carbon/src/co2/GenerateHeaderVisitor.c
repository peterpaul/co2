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
#include "co2/GenerateHeaderVisitor.h"
#include "co2/CompileObject.h"
#include "co2/ClassDeclaration.h"
#include "co2/VariableDeclaration.h"
#include "co2/FunctionDeclaration.h"
#include "co2/InterfaceDeclaration.h"
#include "co2/ConstructorDeclaration.h"
#include "co2/DestructorDeclaration.h"
#include "co2/StructDeclaration.h"
#include "co2/TypeDeclaration.h"
#include "co2/FunctionType.h"
#include "co2/ObjectType.h"
#include "co2/Expression.h"
#include "co2/io.h"

int new_constructor_filter (void *_constructor);
void generate_superclass (struct ClassDeclaration *self);
void FunctionDeclaration_generate_formal_arg (void *_decl, va_list * ap);

static void
ClassDeclaration_generate_constructor_registration (void *_constructor_decl,
						    va_list * app)
{
  struct ConstructorDeclaration *constructor_decl =
    O_CAST (_constructor_decl, ConstructorDeclaration ());
  struct ClassDeclaration *class_decl = O_GET_ARG (ClassDeclaration);
  fprintf (out, "; \\\n O_METHOD (");
  O_CALL (class_decl->name, generate);
  fprintf (out, ", ");
  if (strcmp (constructor_decl->name->name->data, "ctor") != 0)
    {
      fprintf (out, "ctor_");
    }
  O_CALL (constructor_decl->name, generate);
  fprintf (out, ")");
}

static void
ClassDeclaration_generate_method_registration (void *_method_decl,
					       va_list * app)
{
  struct FunctionDeclaration *method_decl =
    O_CAST (_method_decl, FunctionDeclaration ());
  struct ClassDeclaration *class_decl = O_GET_ARG (ClassDeclaration);
  fprintf (out, "; \\\n O_METHOD (");
  if (method_decl->interface_decl)
    {
      O_CALL (method_decl->interface_decl->name, generate);
    }
  else
    {
      O_CALL (class_decl->name, generate);
    }
  fprintf (out, ", ");
  O_CALL (method_decl->name, generate);
  fprintf (out, ")");
}

static void
ClassDeclaration_generate_attribute_registration (void *_method_decl,
						  va_list * app)
{
  struct VariableDeclaration *method_decl =
    O_CAST (_method_decl, VariableDeclaration ());
  struct ClassDeclaration *class_decl = O_GET_ARG (ClassDeclaration);
  fprintf (out, "; \\\n ");
  O_CALL (method_decl->type, generate);
  fprintf (out, " ");
  O_CALL (method_decl->name, generate);
}

static void
InterfaceDeclaration_generate_method_registration (void *_method_decl,
						   va_list * app)
{
  struct FunctionDeclaration *method_decl =
    O_CAST (_method_decl, FunctionDeclaration ());
  struct InterfaceDeclaration *class_decl =
    O_CAST (va_arg (*app, struct InterfaceDeclaration *),
	    InterfaceDeclaration ());
  fprintf (out, "; \\\n O_METHOD (");
  O_CALL (class_decl->name, generate);
  fprintf (out, ", ");
  O_CALL (method_decl->name, generate);
  fprintf (out, ")");
}

#define O_SUPER BaseCompileObjectVisitor()

O_IMPLEMENT(GenerateHeaderVisitor, void *, ctor, (void *_self, va_list *app))
{
  struct GenerateHeaderVisitor * self = O_CAST(_self, GenerateHeaderVisitor());
  self = O_SUPER->ctor(self, app);
  self->fp = va_arg (*app, FILE *);
  current_context = O_CALL_CLASS (Context (), new);
  O_BRANCH_CALL (current_context, retain);
  return self;
}

O_IMPLEMENT(GenerateHeaderVisitor, void *, dtor, (void *_self))
{
  struct GenerateHeaderVisitor *self = O_CAST(_self, GenerateHeaderVisitor());
  O_BRANCH_CALL (current_context, release);
  current_context = NULL;
  return O_SUPER->dtor(self);
}

// O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visit, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitArgumentDeclaration, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitClassDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct ClassDeclaration *self = O_CAST(_object, ClassDeclaration ());

  /* filter the members */
  struct RefList *attributes =
    O_CALL (self->members, filter_args, type_filter, VariableDeclaration ());
  O_CALL (attributes, retain);
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

  struct RefList *new_methods =
    O_CALL (methods, filter_args, Declaration_new_member_filter,
	    FunctionDeclaration ());
  O_CALL (new_methods, retain);

  struct RefList *new_constructors =
    O_CALL (constructors, filter, new_constructor_filter);
  O_CALL (new_constructors, retain);

  /* generate the class */
  fprintf (out, "#define ");
  O_CALL (self->name, generate);
  fprintf (out, "Class_Attr\\\n ");
  generate_superclass (self);
  fprintf (out, "Class_Attr");
  O_CALL (new_constructors, map_args,
	  ClassDeclaration_generate_constructor_registration, self);
  O_CALL (new_methods, map_args,
	  ClassDeclaration_generate_method_registration, self);
  fprintf (out, "\n\n");

  fprintf (out, "#define ");
  O_CALL (self->name, generate);
  fprintf (out, "_Attr\\\n ");
  generate_superclass (self);
  fprintf (out, "_Attr");
  O_CALL (attributes, map_args,
	  ClassDeclaration_generate_attribute_registration, self);
  fprintf (out, "\n\n");

  fprintf (out, "O_CLASS (");
  O_CALL (self->name, generate);
  fprintf (out, ", ");
  generate_superclass (self);
  fprintf (out, ");\n\n");

  O_CALL (attributes, release);
  O_CALL (methods, release);
  O_CALL (new_methods, release);
  O_CALL (constructors, release);
  O_CALL (destructors, release);
}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitConstructorDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateHeaderVisitor *visitor = O_CAST (_self, GenerateHeaderVisitor ());
  struct ConstructorDeclaration *self =
    O_CAST (_object, ConstructorDeclaration ());
  if (new_constructor_filter (self))
    {
      struct ClassDeclaration *class_decl = O_CALL (current_context, find, ClassDeclaration ());
      fprintf (out, "O_METHOD_DEF (");
      O_CALL (class_decl->name, generate);
      fprintf (out, ", void *, ");
      if (strcmp (self->name->name->data, "ctor") != 0)
	{
	  fprintf (out, "ctor_");
	}
      O_CALL (self->name, generate);
      fprintf (out, ", (void *_self, va_list *app));\n\n");
    }
}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct GenerateHeaderVisitor *visitor = O_CAST (_self, GenerateHeaderVisitor ());
  struct Declaration *self = O_CAST (_object, Declaration ());
  if (self->include_file)
    {
      return;
    }
  else
    {
      O_SUPER->visitDeclaration(visitor, self);
      self->defined = true;
      self->declared = true;
    }
}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitDestructorDeclaration, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitFunctionDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct FunctionDeclaration *self = O_CAST(_object, FunctionDeclaration ());
  struct ClassDeclaration *class_decl = O_CALL (current_context, find, ClassDeclaration ());
  struct InterfaceDeclaration *interface_decl = O_CALL (current_context, find, InterfaceDeclaration ());
  if (class_decl)
    {
      if (class_decl->include_file)
	{
	  return;
	}

      if (self->interface_decl)
	{
	  return;
	}
      
      struct FunctionType *method_type =
	o_cast (self->type, FunctionType ());
      fprintf (out, "O_METHOD_DEF (");
      O_CALL (class_decl->name, generate);
      fprintf (out, ", ");
      O_CALL (method_type->return_type, generate);
      fprintf (out, ", ");
      O_CALL (self->name, generate);
      fprintf (out, ", (void *_self");
      O_CALL (self->formal_arguments, map,
	      ObjectTypeDeclaration_generate_method_arguments);
      fprintf (out, "));\n");
    }
  else if (interface_decl)
    {
      if (interface_decl->include_file)
	{
	  return;
	}
      struct FunctionType *method_type =
	o_cast (self->type, FunctionType ());
      fprintf (out, "O_METHOD_DEF (");
      O_CALL (interface_decl->name, generate);
      fprintf (out, ", ");
      O_CALL (method_type->return_type, generate);
      fprintf (out, ", ");
      O_CALL (self->name, generate);
      fprintf (out, ", (void *_self");
      O_CALL (self->formal_arguments, map,
	      ObjectTypeDeclaration_generate_method_arguments);
      fprintf (out, "));\n");
    }
  else
    {
      bool first_formal_arg = true;
      struct FunctionType *function_type = o_cast (self->type, FunctionType ());
      O_CALL (function_type->return_type, generate);
      fprintf (out, " ");
      O_CALL (self->name, generate);
      fprintf (out, " (");
      O_CALL (self->formal_arguments, map_args,
	      FunctionDeclaration_generate_formal_arg, &first_formal_arg);
      fprintf (out, ");\n");
    }
}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitInterfaceDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct InterfaceDeclaration *self = O_CAST (_object, InterfaceDeclaration ());
  /* filter the members */
  struct RefList *methods =
    O_CALL (self->members, filter_args, type_filter, FunctionDeclaration ());
  O_CALL (methods, retain);
  struct RefList *new_methods =
    O_CALL (methods, filter_args, Declaration_new_member_filter,
	    FunctionDeclaration ());
  O_CALL (new_methods, retain);

  fprintf (out, "#include \"co2/Interface.h\"\n");

  /* generate the class */
  fprintf (out, "#define ");
  O_CALL (self->name, generate);
  fprintf (out, "Class_Attr\\\n ");
  fprintf (out, "InterfaceClass_Attr");
  fprintf (out, "\n\n");

  fprintf (out, "#define ");
  O_CALL (self->name, generate);
  fprintf (out, "_Attr\\\n ");
  fprintf (out, "Interface_Attr");
  O_CALL (new_methods, map_args,
	  InterfaceDeclaration_generate_method_registration, self);
  fprintf (out, "\n\n");

  fprintf (out, "O_CLASS (");
  O_CALL (self->name, generate);
  fprintf (out, ", Interface);\n\n");

}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitMacroDeclaration, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitStructDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct StructDeclaration *self = O_CAST (_object, StructDeclaration ());
  fprintf (out, "struct ");
  O_CALL (self->name, generate);
  fprintf (out, " {\n");
  O_CALL (self->members, map, CompileObject_generate);
  fprintf (out, "};\n\n");
}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitTypeDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct TypeDeclaration *self = O_CAST(_object, TypeDeclaration());
  fprintf (out, "typedef ");
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
  fprintf (out, ";\n");
}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitVariableDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct VariableDeclaration *self = O_CAST(_object, VariableDeclaration());

  if (self->scope && self->scope->type != GLOBAL_SCOPE)
    {
      // only generate global declarations
      return;
    }
  fprintf (out, "extern ");
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
  fprintf (out, ";\n");
}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitCatchStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitCompoundStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitDeleteStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitDoStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitExpressionStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitForEachStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitForStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitIfStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitReturnStatement, (void *_self, void *_object), (_self, _object)) {}
// O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitThrowStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitTryStatement, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitWhileStatement, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitBinaryExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitCastExpression, (void *_self, void *_object), (_self, _object)) {}
// O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitFunctionCallExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitNestedExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitNewExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitNullExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitSizeExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitSuperExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitTokenExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitUnaryExpression, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitVarArgExpression, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitArrayType, (void *_self, void *_object), (_self, _object)) {}
O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitFunctionType, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitObjectType, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct ObjectType *self = O_CAST (_object, ObjectType ());

  if (self->decl && !self->decl->defined)
    {
      fprintf (out, "struct ");
      O_CALL (self->token, generate);
      fprintf (out, ";\n");
      self->decl->defined = true;
    }
}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitPrimitiveType, (void *_self, void *_object), (_self, _object)) {}
// O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitType, (void *_self, void *_object), (_self, _object)) {}

O_IMPLEMENT_IF(GenerateHeaderVisitor, void, visitFile, (void *_self, void *_object), (_self, _object)) {}


O_OBJECT(GenerateHeaderVisitor, BaseCompileObjectVisitor);
O_OBJECT_METHOD(GenerateHeaderVisitor, ctor);
O_OBJECT_METHOD(GenerateHeaderVisitor, dtor);

// O_OBJECT_METHOD (GenerateHeaderVisitor, visit);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitArgumentDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitClassDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitConstructorDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitDestructorDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitFunctionDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitInterfaceDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitMacroDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitStructDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitTypeDeclaration);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitVariableDeclaration);

O_OBJECT_METHOD (GenerateHeaderVisitor, visitCatchStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitCompoundStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitDeleteStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitDoStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitExpressionStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitForEachStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitForStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitIfStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitReturnStatement);
// O_OBJECT_METHOD (GenerateHeaderVisitor, visitStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitThrowStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitTryStatement);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitWhileStatement);

O_OBJECT_METHOD (GenerateHeaderVisitor, visitBinaryExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitCastExpression);
// O_OBJECT_METHOD (GenerateHeaderVisitor, visitExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitFunctionCallExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitNestedExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitNewExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitNullExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitSizeExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitSuperExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitTokenExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitUnaryExpression);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitVarArgExpression);

O_OBJECT_METHOD (GenerateHeaderVisitor, visitArrayType);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitFunctionType);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitObjectType);
O_OBJECT_METHOD (GenerateHeaderVisitor, visitPrimitiveType);
// O_OBJECT_METHOD (GenerateHeaderVisitor, visitType);

O_OBJECT_METHOD (GenerateHeaderVisitor, visitFile);

O_OBJECT_IF(CompileObjectVisitor);
// O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visit);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitArgumentDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitClassDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitConstructorDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitDestructorDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitFunctionDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitInterfaceDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitMacroDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitStructDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitTypeDeclaration);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitVariableDeclaration);

O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitCatchStatement);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitCompoundStatement);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitDeleteStatement);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitDoStatement);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitExpressionStatement);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitForEachStatement);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitForStatement);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitIfStatement);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitReturnStatement);
// O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitStatement);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitThrowStatement);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitTryStatement);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitWhileStatement);

O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitBinaryExpression);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitCastExpression);
// O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitExpression);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitFunctionCallExpression);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitNestedExpression);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitNewExpression);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitNullExpression);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitSizeExpression);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitSuperExpression);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitTokenExpression);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitUnaryExpression);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitVarArgExpression);

O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitArrayType);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitFunctionType);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitObjectType);
O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitPrimitiveType);
// O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitType);

O_OBJECT_IF_METHOD (GenerateHeaderVisitor, visitFile);
O_OBJECT_IF_END
O_END_OBJECT
