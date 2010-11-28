#include "SuperExpression.h"
#include "Token.h"
#include "FunctionDeclaration.h"
#include "ClassDeclaration.h"
#include "ConstructorDeclaration.h"
#include "ArgumentDeclaration.h"
#include "Type.h"
#include "grammar.tab.h"
#include "io.h"

#define O_SUPER Expression()

O_IMPLEMENT (SuperExpression, void *, ctor, (void *_self, va_list * app))
{
  struct SuperExpression *self = O_CAST (_self, SuperExpression ());
  self = O_SUPER->ctor (self, app);
  self->super = O_BRANCH_CAST (va_arg (*app, struct Token *), Token ());
  self->ctor_name = O_BRANCH_CAST (va_arg (*app, struct Token *), Token ());
  self->actual_arguments =
    O_BRANCH_CAST (va_arg (*app, struct RefList *), RefList ());
  O_BRANCH_CALL (self->super, retain);
  O_BRANCH_CALL (self->ctor_name, retain);
  O_BRANCH_CALL (self->actual_arguments, retain);
  return self;
}

O_IMPLEMENT (SuperExpression, void *, dtor, (void *_self))
{
  struct SuperExpression *self = O_CAST (_self, SuperExpression ());
  O_BRANCH_CALL (self->super, release);
  O_BRANCH_CALL (self->ctor_name, release);
  O_BRANCH_CALL (self->actual_arguments, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (SuperExpression, void, type_check, (void *_self))
{
  struct SuperExpression *self = O_CAST (_self, SuperExpression ());
  /* get class context */
  self->class_context = O_CALL (current_context, find, ClassDeclaration ());
  /* get method context */
  self->method_context =
    O_CALL (current_context, find, ConstructorDeclaration ());;
  if (self->method_context == NULL)
    {
      self->method_context =
	O_CALL (current_context, find, FunctionDeclaration ());
    }

  struct ClassDeclaration *super =
    (struct ClassDeclaration *) O_CALL (global_scope, lookup,
					self->class_context->superclass);

  if (o_is_of (self->method_context, ConstructorDeclaration ()))
    {
      struct Token *ctor_name = self->ctor_name;
      if (ctor_name == NULL)
	{
	  ctor_name =
	    O_CALL_CLASS (Token (), new, "ctor", IDENTIFIER, filename,
			  self->super->line);
	}
      O_CALL (ctor_name, retain);
      struct ConstructorDeclaration *ctor_decl =
	(struct ConstructorDeclaration *) O_CALL (super->member_scope, lookup,
						  ctor_name);

      if (self->actual_arguments->length <
	  ctor_decl->formal_arguments->length)
	{
	  error (self->super, "'%s' needs %d arguments, but got %d.\n",
		 ctor_decl->name->name->data,
		 ctor_decl->formal_arguments->length,
		 self->actual_arguments->length);
	  return;
	}
      int i;
      for (i = 0; i < ctor_decl->formal_arguments->length; i++)
	{
	  struct ArgumentDeclaration *arg_decl =
	    O_CALL (ctor_decl->formal_arguments, get, i);
	  struct Expression *arg_expr =
	    O_CALL (self->actual_arguments, get, i);
	  O_CALL (arg_expr, type_check);
	  O_CALL (arg_decl->type, assert_compatible, arg_expr->type);
	}
      O_CALL (ctor_name, release);
    }
  else if (o_is_of (self->method_context, FunctionDeclaration ()))
    {
      struct FunctionDeclaration *function_decl =
	(struct FunctionDeclaration *) O_CALL (super->member_scope, lookup,
					       self->method_context->name);

      if (self->actual_arguments->length <
	  function_decl->formal_arguments->length)
	{
	  error (self->super, "'%s' needs %d arguments, but got %d.\n",
		 function_decl->name->name->data,
		 function_decl->formal_arguments->length,
		 self->actual_arguments->length);
	  struct Declaration *null_ptr_gen = NULL;
	  null_ptr_gen->name = NULL;
	}
      int i;
      for (i = 0; i < function_decl->formal_arguments->length; i++)
	{
	  struct ArgumentDeclaration *arg_decl =
	    O_CALL (function_decl->formal_arguments, get, i);
	  struct Expression *arg_expr =
	    O_CALL (self->actual_arguments, get, i);
	  O_CALL (arg_expr, type_check);
	  O_CALL (arg_decl->type, assert_compatible, arg_expr->type);
	}
    }
  else
    {
      error (self->super, "No context found for super keyword.\n");
    }
}

O_IMPLEMENT (SuperExpression, void, generate, (void *_self))
{
  struct SuperExpression *self = O_CAST (_self, SuperExpression ());

  if (o_is_of (self->method_context, ConstructorDeclaration ()))
    {
      if (self->ctor_name == NULL)
	{
	  bool is_first_arg = false;
	  fprintf (out, "self = o_super_ctor(self, O_SUPER");
	  O_CALL (self->actual_arguments, map_args,
		  Expression_generate_actual_argument, &is_first_arg);
	  fprintf (out, ")");
	}
      else
	{
	  bool is_first_arg = false;
	  fprintf (out, "self = o_super_ctor_named(self, _");
	  O_CALL (self->class_context->superclass, generate);
	  fprintf (out, "_ctor_");
	  O_CALL (self->ctor_name, generate);
	  O_CALL (self->actual_arguments, map_args,
		  Expression_generate_actual_argument, &is_first_arg);
	  fprintf (out, ")");
	}
    }
  else
    {
      bool is_first_arg = false;
      fprintf (out, "O_SUPER->");
      O_CALL (self->method_context->name, generate);
      fprintf (out, "(self");
      O_CALL (self->actual_arguments, map_args,
	      Expression_generate_actual_argument, &is_first_arg);
      fprintf (out, ")");
    }
}

O_OBJECT (SuperExpression, Expression);
O_OBJECT_METHOD (SuperExpression, ctor);
O_OBJECT_METHOD (SuperExpression, dtor);
O_OBJECT_METHOD (SuperExpression, generate);
O_OBJECT_METHOD (SuperExpression, type_check);
O_END_OBJECT
