#include "GenerateSourceVisitor.h"
#include "CompileObject.h"
#include "ArgumentDeclaration.h"
#include "ClassDeclaration.h"
#include "VariableDeclaration.h"
#include "FunctionDeclaration.h"
#include "InterfaceDeclaration.h"
#include "ConstructorDeclaration.h"
#include "DestructorDeclaration.h"
#include "StructDeclaration.h"
#include "TypeDeclaration.h"
#include "FunctionType.h"
#include "Statement.h"
#include "PrimitiveType.h"
#include "grammar.tab.h"
#include "Expression.h"
#include "FunctionType.h"
#include "ObjectType.h"
#include "Type.h"
#include "io.h"

int new_constructor_filter (void *_constructor);
void FunctionDeclaration_generate_formal_arg (void *_decl, va_list * ap);


static void
ClassDeclaration_generate_constructor_arguments (void *_arg)
{
  struct ArgumentDeclaration *arg = O_CAST (_arg, ArgumentDeclaration ());
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

static void
ClassDeclaration_generate_constructor_registration_2 (void *_constructor_decl,
						      va_list * app)
{
  struct ConstructorDeclaration *constructor_decl =
    O_CAST (_constructor_decl, ConstructorDeclaration ());
  struct ClassDeclaration *class_decl = O_GET_ARG (ClassDeclaration);
  fprintf (out, "O_OBJECT_METHOD (");
  O_CALL (class_decl->name, generate);
  fprintf (out, ", ");
  if (strcmp (constructor_decl->name->name->data, "ctor") != 0)
    {
      fprintf (out, "ctor_");
    }
  O_CALL (constructor_decl->name, generate);
  fprintf (out, ");\n");
}

static void
ClassDeclaration_generate_destructor_registration_2 (void *_destructor_decl,
						     va_list * app)
{
  struct DestructorDeclaration *destructor_decl =
    O_CAST (_destructor_decl, DestructorDeclaration ());
  struct ClassDeclaration *class_decl = O_GET_ARG (ClassDeclaration);
  fprintf (out, "O_OBJECT_METHOD (");
  O_CALL (class_decl->name, generate);
  fprintf (out, ", dtor);\n");
}

static void
ClassDeclaration_generate_interface_method_registration (void *_method_decl,
							 va_list * app)
{
  struct Declaration *method_decl = O_CAST (_method_decl, Declaration ());
  struct Token *token = O_GET_ARG (Token);

  fprintf (out, "O_OBJECT_IF_METHOD (");
  O_CALL (token, generate);
  fprintf (out, ", ");
  O_CALL (method_decl->name, generate);
  fprintf (out, ");\n");
}

static void
ClassDeclaration_generate_method_implementation_2 (void *_interface_name,
						   va_list * app)
{
  struct Token *interface_name = O_CAST (_interface_name, Token ());
  struct Token *implementation_name = O_GET_ARG (Token);
  struct Declaration *_decl =
    O_CALL (global_scope, lookup_in_this_scope, interface_name);
  struct InterfaceDeclaration *interface_decl =
    O_CAST (_decl, InterfaceDeclaration ());

  fprintf (out, "O_OBJECT_IF (");
  O_CALL (interface_name, generate);
  fprintf (out, ");\n");
  O_CALL (interface_decl->members, map_args,
	  ClassDeclaration_generate_interface_method_registration,
	  implementation_name);
  fprintf (out, "O_OBJECT_IF_END\n");
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

// O_IMPLEMENT_IF(GenerateSourceVisitor, void, visit, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitArgumentDeclaration, (void *_self, void *object), (_self, object)) {}

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

  fprintf (out, "#define O_SUPER ");
  generate_superclass (self);
  fprintf (out, " ()\n\n");

  fprintf (out, "O_OBJECT (");
  O_CALL (self->name, generate);
  fprintf (out, ", ");
  generate_superclass (self);
  fprintf (out, ");\n");

  O_CALL (constructors, map_args,
	  ClassDeclaration_generate_constructor_registration_2, self);
  O_CALL (destructors, map_args,
	  ClassDeclaration_generate_destructor_registration_2, self);
  O_CALL (methods, map_args,
	  ObjectTypeDeclaration_generate_method_registration_2, self);

  if (self->interfaces)
    {
      O_CALL (self->interfaces, map_args,
	      ClassDeclaration_generate_method_implementation_2, self->name);
    }

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
      fprintf (out, "#include ");
      O_CALL (self->include_file, generate);
      fprintf (out, "\n");
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

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitFunctionDeclaration, (void *_self, void *_object), (_self, _object))
{
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
      struct FunctionType *method_type =
	o_cast (self->type, FunctionType ());
      if (self->interface_decl)
	{
	  fprintf (out, "O_IMPLEMENT_IF (");
	}
      else
	{
	  fprintf (out, "O_IMPLEMENT (");
	}
      O_CALL (class_decl->name, generate);
      fprintf (out, ", ");
      O_CALL (method_type->return_type, generate);
      fprintf (out, ", ");
      O_CALL (self->name, generate);
      fprintf (out, ", (void *_self");
      O_CALL (self->formal_arguments, map,
	      ObjectTypeDeclaration_generate_method_arguments);
      if (self->interface_decl)
	{
	  fprintf (out, "), (_self");
	  O_CALL (self->formal_arguments, map,
		  ObjectTypeDeclaration_generate_method_argument_names);
	}
      fprintf (out, "))\n");
      fprintf (out, "{\n");
      fprintf (out, "struct ");
      O_CALL (class_decl->name, generate);
      fprintf (out, "* self = O_CAST (_self, ");
      O_CALL (class_decl->name, generate);
      fprintf (out, " ());\n");
      
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

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitMacroDeclaration, (void *_self, void *object), (_self, object)) {}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitStructDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct StructDeclaration *self = O_CAST (_object, StructDeclaration ());
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitTypeDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct TypeDeclaration *self = O_CAST(_object, TypeDeclaration());
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitVariableDeclaration, (void *_self, void *_object), (_self, _object))
{
  struct BaseCompileObjectVisitor *visitor = O_CAST(_self, BaseCompileObjectVisitor());
  struct VariableDeclaration *self = O_CAST(_object, VariableDeclaration());

  if (self->scope && self->scope->type != GLOBAL_SCOPE)
    {
      // only generate global declarations
      return;
    }

  O_CALL (self->type, generate);
  fprintf (out, " ");
  O_CALL (self->name, generate);
  if (self->expr)
    {
      fprintf (out, " = ");
      O_CALL (self->expr, generate);
    }
  fprintf (out, ";\n");
}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitCatchStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitCompoundStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitDeleteStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitDoStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitExpressionStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitForEachStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitForStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitIfStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitReturnStatement, (void *_self, void *object), (_self, object)) {}
// O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitThrowStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitTryStatement, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitWhileStatement, (void *_self, void *object), (_self, object)) {}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitBinaryExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitCastExpression, (void *_self, void *object), (_self, object)) {}
// O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitFunctionCallExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitNestedExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitNewExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitNullExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitSizeExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitSuperExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitTokenExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitUnaryExpression, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitVarArgExpression, (void *_self, void *object), (_self, object)) {}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitArrayType, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitFunctionType, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitObjectType, (void *_self, void *object), (_self, object)) {}
O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitPrimitiveType, (void *_self, void *object), (_self, object)) {}
// O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitType, (void *_self, void *object), (_self, object)) {}

O_IMPLEMENT_IF(GenerateSourceVisitor, void, visitFile, (void *_self, void *object), (_self, object)) {}


O_OBJECT(GenerateSourceVisitor, BaseCompileObjectVisitor);
O_OBJECT_METHOD(GenerateSourceVisitor, ctor);
O_OBJECT_METHOD(GenerateSourceVisitor, dtor);

// O_OBJECT_METHOD (GenerateSourceVisitor, visit);
O_OBJECT_METHOD (GenerateSourceVisitor, visitArgumentDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitClassDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitConstructorDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitDestructorDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitFunctionDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitInterfaceDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitMacroDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitStructDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitTypeDeclaration);
O_OBJECT_METHOD (GenerateSourceVisitor, visitVariableDeclaration);

O_OBJECT_METHOD (GenerateSourceVisitor, visitCatchStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitCompoundStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitDeleteStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitDoStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitExpressionStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitForEachStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitForStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitIfStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitReturnStatement);
// O_OBJECT_METHOD (GenerateSourceVisitor, visitStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitThrowStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitTryStatement);
O_OBJECT_METHOD (GenerateSourceVisitor, visitWhileStatement);

O_OBJECT_METHOD (GenerateSourceVisitor, visitBinaryExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitCastExpression);
// O_OBJECT_METHOD (GenerateSourceVisitor, visitExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitFunctionCallExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitNestedExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitNewExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitNullExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitSizeExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitSuperExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitTokenExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitUnaryExpression);
O_OBJECT_METHOD (GenerateSourceVisitor, visitVarArgExpression);

O_OBJECT_METHOD (GenerateSourceVisitor, visitArrayType);
O_OBJECT_METHOD (GenerateSourceVisitor, visitFunctionType);
O_OBJECT_METHOD (GenerateSourceVisitor, visitObjectType);
O_OBJECT_METHOD (GenerateSourceVisitor, visitPrimitiveType);
// O_OBJECT_METHOD (GenerateSourceVisitor, visitType);

O_OBJECT_METHOD (GenerateSourceVisitor, visitFile);

O_OBJECT_IF(CompileObjectVisitor);
O_OBJECT_IF_METHOD(GenerateSourceVisitor, visitClassDeclaration);
O_OBJECT_IF_END
O_END_OBJECT
