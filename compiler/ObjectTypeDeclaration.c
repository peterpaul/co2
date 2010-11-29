#include "ObjectTypeDeclaration.h"
#include "FunctionDeclaration.h"
#include "ArgumentDeclaration.h"
#include "VariableDeclaration.h"
#include "ConstructorDeclaration.h"
#include "DestructorDeclaration.h"
#include "InterfaceDeclaration.h"
#include "Type.h"
#include "ObjectType.h"
#include "Token.h"
#include "RefList.h"
#include "FunctionType.h"
#include "io.h"
#include "Statement.h"
#include "Scope.h"
#include "PrimitiveType.h"
#include "grammar.tab.h"

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
}

int
ObjectTypeDeclaration_new_constructor_filter (void *_constructor)
{
  struct ConstructorDeclaration *constructor =
    O_CAST (_constructor, ConstructorDeclaration ());
  if (strcmp (constructor->name->name->data, "ctor") != 0)
    {
      return O_BRANCH_CALL (constructor->scope->parent,
			    lookup_type_in_this_scope, constructor->name,
			    ConstructorDeclaration ()) == NULL;
    }
  return false;
}

void
ObjectTypeDeclaration_generate_constructor_arguments (void *_arg)
{
  struct ArgumentDeclaration *arg = O_CAST (_arg, ArgumentDeclaration ());
  O_CALL (arg, generate);
  if (o_is_a (arg->type, ObjectType ()))
    {
      struct ObjectType *type = (struct ObjectType *) arg->type;
      fprintf (out, " = O_BRANCH_CAST (va_arg (*app, ");
      O_CALL (type, generate);
      fprintf (out, "), ");
      O_CALL (type->token, generate);
      fprintf (out, " ());\n");
    }
  else
    {
      fprintf (out, " = va_arg (*app, ");
      O_CALL (arg->type, generate);
      fprintf (out, ");\n");
    }
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
ObjectTypeDeclaration_generate_constructor_definition (void
						       *_constructor_decl,
						       va_list * app)
{
  struct ConstructorDeclaration *constructor_decl =
    O_CAST (_constructor_decl, ConstructorDeclaration ());
  struct ObjectTypeDeclaration *class_decl =
    O_CAST (va_arg (*app, struct ObjectTypeDeclaration *),
	    ObjectTypeDeclaration ());
  fprintf (out, "O_METHOD_DEF (");
  O_CALL (class_decl->name, generate);
  fprintf (out, ", void *, ");
  if (strcmp (constructor_decl->name->name->data, "ctor") != 0)
    {
      fprintf (out, "ctor_");
    }
  O_CALL (constructor_decl->name, generate);
  fprintf (out, ", (void *_self, va_list *app));\n");
}

void
ObjectTypeDeclaration_generate_method_definition (void *_method_decl,
						  va_list * app)
{
  struct FunctionDeclaration *method_decl =
    O_CAST (_method_decl, FunctionDeclaration ());
  if (method_decl->interface_decl)
    {
      return;
    }

  struct ObjectTypeDeclaration *class_decl =
    O_CAST (va_arg (*app, struct ObjectTypeDeclaration *),
	    ObjectTypeDeclaration ());
  struct FunctionType *method_type =
    o_cast (method_decl->type, FunctionType ());
  fprintf (out, "O_METHOD_DEF (");
  O_CALL (class_decl->name, generate);
  fprintf (out, ", ");
  O_CALL (method_type->return_type, generate);
  fprintf (out, ", ");
  O_CALL (method_decl->name, generate);
  fprintf (out, ", (void *_self");
  O_CALL (method_decl->formal_arguments, map,
	  ObjectTypeDeclaration_generate_method_arguments);
  fprintf (out, "));\n");
}

void
ObjectTypeDeclaration_generate_constructor_registration (void
							 *_constructor_decl,
							 va_list * app)
{
  struct ConstructorDeclaration *constructor_decl =
    O_CAST (_constructor_decl, ConstructorDeclaration ());
  struct ObjectTypeDeclaration *class_decl =
    O_CAST (va_arg (*app, struct ObjectTypeDeclaration *),
	    ObjectTypeDeclaration ());
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

void
ObjectTypeDeclaration_generate_method_registration (void *_method_decl,
						    va_list * app)
{
  struct FunctionDeclaration *method_decl =
    O_CAST (_method_decl, FunctionDeclaration ());
  struct ObjectTypeDeclaration *class_decl =
    O_CAST (va_arg (*app, struct ObjectTypeDeclaration *),
	    ObjectTypeDeclaration ());
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

void
ObjectTypeDeclaration_generate_constructor_registration_2 (void
							   *_constructor_decl,
							   va_list * app)
{
  struct ConstructorDeclaration *constructor_decl =
    O_CAST (_constructor_decl, ConstructorDeclaration ());
  struct ObjectTypeDeclaration *class_decl =
    O_CAST (va_arg (*app, struct ObjectTypeDeclaration *),
	    ObjectTypeDeclaration ());
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

void
ObjectTypeDeclaration_generate_destructor_registration_2 (void
							  *_destructor_decl,
							  va_list * app)
{
  struct DestructorDeclaration *destructor_decl =
    O_CAST (_destructor_decl, DestructorDeclaration ());
  struct ObjectTypeDeclaration *class_decl =
    O_CAST (va_arg (*app, struct ObjectTypeDeclaration *),
	    ObjectTypeDeclaration ());
  fprintf (out, "O_OBJECT_METHOD (");
  O_CALL (class_decl->name, generate);
  fprintf (out, ", dtor);\n");
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
  fprintf (out, "O_OBJECT_METHOD (");
  O_CALL (class_decl->name, generate);
  fprintf (out, ", ");
  O_CALL (method_decl->name, generate);
  fprintf (out, ");\n");
}

void
ObjectTypeDeclaration_generate_interface_method_registration (void
							      *_method_decl,
							      va_list * app)
{
  struct Declaration *method_decl = O_CAST (_method_decl, Declaration ());
  struct Token *token = O_CAST (va_arg (*app, struct Token *), Token ());

  fprintf (out, "O_OBJECT_IF_METHOD (");
  O_CALL (token, generate);
  fprintf (out, ", ");
  O_CALL (method_decl->name, generate);
  fprintf (out, ");\n");
}

void
ObjectTypeDeclaration_generate_method_implementation_2 (void *_interface_name,
							va_list * app)
{
  struct Token *interface_name = O_CAST (_interface_name, Token ());
  struct Token *implementation_name =
    O_CAST (va_arg (*app, struct Token *), Token ());
  struct Declaration *_decl =
    O_CALL (global_scope, lookup_in_this_scope, interface_name);
  struct InterfaceDeclaration *interface_decl =
    O_CAST (_decl, InterfaceDeclaration ());

  fprintf (out, "O_OBJECT_IF (");
  O_CALL (interface_name, generate);
  fprintf (out, ");\n");
  O_CALL (interface_decl->members, map_args,
	  ObjectTypeDeclaration_generate_interface_method_registration,
	  implementation_name);
  fprintf (out, "O_OBJECT_IF_END\n");
}

void
ObjectTypeDeclaration_generate_constructor_implementation (void
							   *_constructor_decl,
							   va_list * app)
{
  struct ConstructorDeclaration *constructor_decl =
    O_CAST (_constructor_decl, ConstructorDeclaration ());
  struct ObjectTypeDeclaration *class_decl =
    O_CAST (va_arg (*app, struct ObjectTypeDeclaration *),
	    ObjectTypeDeclaration ());
  fprintf (out, "O_IMPLEMENT (");
  O_CALL (class_decl->name, generate);
  fprintf (out, ", void *");
  fprintf (out, ", ");
  if (strcmp (constructor_decl->name->name->data, "ctor") != 0)
    {
      fprintf (out, "ctor_");
    }
  O_CALL (constructor_decl->name, generate);
  fprintf (out, ", (void *_self, va_list *app))\n");
  fprintf (out, "{\n");
  fprintf (out, "struct ");
  O_CALL (class_decl->name, generate);
  fprintf (out, "* self = O_CAST (_self, ");
  O_CALL (class_decl->name, generate);
  fprintf (out, " ());\n");
  O_CALL (constructor_decl->formal_arguments, map,
	  ObjectTypeDeclaration_generate_constructor_arguments);

  O_CALL (constructor_decl->body, generate);
  fprintf (out, "return self;\n");
  fprintf (out, "}\n\n");
}

void
ObjectTypeDeclaration_generate_destructor_implementation (void
							  *_destructor_decl,
							  va_list * app)
{
  struct DestructorDeclaration *destructor_decl =
    O_CAST (_destructor_decl, DestructorDeclaration ());
  struct ObjectTypeDeclaration *class_decl =
    O_CAST (va_arg (*app, struct ObjectTypeDeclaration *),
	    ObjectTypeDeclaration ());
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
  O_CALL (destructor_decl->body, generate);
  fprintf (out, "return O_SUPER->dtor (self);\n");
  fprintf (out, "}\n\n");
}

void
ObjectTypeDeclaration_generate_method_implementation (void *_method_decl,
						      va_list * app)
{
  struct FunctionDeclaration *method_decl =
    O_CAST (_method_decl, FunctionDeclaration ());
  struct ObjectTypeDeclaration *class_decl =
    O_CAST (va_arg (*app, struct ObjectTypeDeclaration *),
	    ObjectTypeDeclaration ());
  struct FunctionType *method_type =
    o_cast (method_decl->type, FunctionType ());
  if (method_decl->interface_decl)
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
  O_CALL (method_decl->name, generate);
  fprintf (out, ", (void *_self");
  O_CALL (method_decl->formal_arguments, map,
	  ObjectTypeDeclaration_generate_method_arguments);
  if (method_decl->interface_decl)
    {
      fprintf (out, "), (_self");
      O_CALL (method_decl->formal_arguments, map,
	      ObjectTypeDeclaration_generate_method_argument_names);
    }
  fprintf (out, "))\n");
  fprintf (out, "{\n");
  fprintf (out, "struct ");
  O_CALL (class_decl->name, generate);
  fprintf (out, "* self = O_CAST (_self, ");
  O_CALL (class_decl->name, generate);
  fprintf (out, " ());\n");

  if (!o_is_of (method_type->return_type, PrimitiveType ()) ||
      ((struct PrimitiveType *) (method_type->return_type))->token->type !=
      VOID)
    {
      O_CALL (method_type->return_type, generate);
      fprintf (out, " return_value;\n");
    }

  if (method_type->has_var_args)
    {
      fprintf (out, "va_list ap;\n");
      fprintf (out, "va_start (ap, ");
      struct ArgumentDeclaration *arg_decl;
      if (method_decl->formal_arguments->length == 1)
	{
	  fprintf (out, "_self");
	}
      else
	{
	  struct ArgumentDeclaration *arg_decl =
	    O_CALL (method_decl->formal_arguments, get,
		    method_decl->formal_arguments->length - 2);
	  O_CALL (arg_decl->name, generate);
	}
      fprintf (out, ");\n");
    }

  O_CALL (method_decl->body, generate);

  if (!o_is_of (method_type->return_type, PrimitiveType ()) ||
      ((struct PrimitiveType *) (method_type->return_type))->token->type !=
      VOID)
    {
      fprintf (out, "function_end:\n");
    }

  if (method_type->has_var_args)
    {
      fprintf (out, "va_end (ap);\n");
    }

  if (!o_is_of (method_type->return_type, PrimitiveType ()) ||
      ((struct PrimitiveType *) (method_type->return_type))->token->type !=
      VOID)
    {
      fprintf (out, "return return_value;\n");
    }

  fprintf (out, "}\n\n");
}

void
ObjectTypeDeclaration_generate_attribute_registration (void *_method_decl,
						       va_list * app)
{
  struct VariableDeclaration *method_decl =
    O_CAST (_method_decl, VariableDeclaration ());
  struct ObjectTypeDeclaration *class_decl =
    O_CAST (va_arg (*app, struct ObjectTypeDeclaration *),
	    ObjectTypeDeclaration ());
  fprintf (out, "; \\\n ");
  O_CALL (method_decl->type, generate);
  fprintf (out, " ");
  O_CALL (method_decl->name, generate);
}

O_OBJECT (ObjectTypeDeclaration, Declaration);
O_OBJECT_METHOD (ObjectTypeDeclaration, ctor);
O_OBJECT_METHOD (ObjectTypeDeclaration, dtor);
O_END_OBJECT
