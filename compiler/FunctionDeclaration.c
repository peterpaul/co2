#include "FunctionDeclaration.h"
#include "Type.h"
#include "RefList.h"
#include "Statement.h"
#include "io.h"
#include "FunctionType.h"
#include "ArgumentDeclaration.h"
#include "ClassDeclaration.h"
#include "InterfaceDeclaration.h"
#include "grammar.tab.h"
#include "PrimitiveType.h"

#define O_SUPER Declaration()

O_IMPLEMENT (FunctionDeclaration, void *, ctor, (void *_self, va_list * app))
{
  struct FunctionDeclaration *self = O_CAST (_self, FunctionDeclaration ());
  self = O_SUPER->ctor (self, app);
  self->type = O_RETAIN_ARG (FunctionType);
  self->formal_arguments = O_RETAIN_ARG (RefList);
  self->body = O_BRANCH_RETAIN_ARG (Statement);
  return self;
}

O_IMPLEMENT (FunctionDeclaration, void *, dtor, (void *_self))
{
  struct FunctionDeclaration *self = O_CAST (_self, FunctionDeclaration ());
  O_CALL (self->formal_arguments, release);
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

static void
FunctionDeclaration_find_in_interface (void *_self, va_list * app)
{
  struct Token *self = O_CAST (_self, Token ());
  struct FunctionDeclaration *function_decl =
    O_CAST (va_arg (*app, struct FunctionDeclaration *),
	    FunctionDeclaration ());
  if (O_CALL (global_scope, exists_in_this_scope, self))
    {
      struct Declaration *_decl =
	O_CALL (global_scope, lookup_in_this_scope, self);
      if (o_is_of (_decl, InterfaceDeclaration ()))
	{
	  function_decl->interface_decl =
	    O_CAST (_decl, InterfaceDeclaration ());
	}
      else
	{
	  error (function_decl->name, "%s is not an interface",
		 _decl->name->name->data);
	}
    }
}

static void FunctionDeclaration_type_check_formal_arg (void *_self)
{
  struct Declaration *self = O_CAST (_self, Declaration ());
  O_CALL (self, type_check);
}

O_IMPLEMENT (FunctionDeclaration, void, type_check, (void *_self))
{
  struct FunctionDeclaration *self = O_CAST (_self, FunctionDeclaration ());
  O_BRANCH_CALL (current_context, add, self);
  O_CALL (self->type, type_check);
  O_CALL (self->formal_arguments, map, FunctionDeclaration_type_check_formal_arg);
  O_BRANCH_CALL (self->body, type_check);

  struct FunctionType *function_type = o_cast (self->type, FunctionType ());
  struct ClassDeclaration *class_decl =
    O_BRANCH_CALL (current_context, find, ClassDeclaration ());
  if (function_type->has_var_args)
    {
      if (class_decl == NULL && self->formal_arguments->length <= 1)
	{
	  error (self->name,
		 "variable argument list only supported on functions with at least one fixed argument.\n");
	}
    }
  if (class_decl && class_decl->interfaces)
    {
      O_CALL (class_decl->interfaces, map_args,
	      FunctionDeclaration_find_in_interface, self);
    }
  O_BRANCH_CALL (current_context, remove_last);
}

O_OBJECT (FunctionDeclaration, Declaration);
O_OBJECT_METHOD (FunctionDeclaration, ctor);
O_OBJECT_METHOD (FunctionDeclaration, dtor);
O_OBJECT_METHOD (FunctionDeclaration, accept);
O_OBJECT_METHOD (FunctionDeclaration, type_check);
O_END_OBJECT
