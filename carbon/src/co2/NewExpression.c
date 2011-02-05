#include "co2/NewExpression.h"
#include "co2/ObjectType.h"
#include "co2/Expression.h"
#include "co2/ArgumentDeclaration.h"
#include "co2/ClassDeclaration.h"
#include "co2/ConstructorDeclaration.h"
#include "co2/RefList.h"
#include "co2/PrimitiveType.h"
#include "co2/ArrayType.h"
#include "co2/io.h"
#include "co2/Token.h"
#include "grammar.h"

#define O_SUPER Expression()

O_IMPLEMENT (NewExpression, void *, ctor, (void *_self, va_list * app))
{
  struct NewExpression *self = O_CAST (_self, NewExpression ());
  self = O_SUPER->ctor (self, app);
  self->new_type = O_RETAIN_ARG (Type);
  struct Object *arg = O_RETAIN_ARG (RefObject);
  if (o_is_of (arg, Expression ()))
    {
      self->array_size = (struct Expression *) arg;
    }
  else if (o_is_of (arg, RefList ()))
    {
      self->ctor_arguments = (struct RefList *) arg;
    }
  else
    {
      error (NULL, "expected expression or list");
    }
  return self;
}

O_IMPLEMENT (NewExpression, void *, dtor, (void *_self))
{
  struct NewExpression *self = O_CAST (_self, NewExpression ());
  O_CALL (self->new_type, release);
  O_BRANCH_CALL (self->ctor_name, release);
  O_BRANCH_CALL (self->array_size, release);
  O_BRANCH_CALL (self->ctor_arguments, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (NewExpression, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct NewExpression *self = O_CAST (_self, NewExpression ());
  O_BRANCH_CALL (self->ctor_name, accept, visitor);
  O_BRANCH_CALL (self->array_size, accept, visitor);
  O_BRANCH_CALL (self->ctor_arguments, map_args, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (NewExpression, void, generate, (void *_self))
{
  struct NewExpression *self = O_CAST (_self, NewExpression ());
  if (self->ctor_arguments)
    {
      bool is_first_arg = false;
      fprintf (out, "O_CALL_CLASS (");
      struct Token *token = O_CALL (self->new_type, get_token);
      O_CALL (token, generate);
      if (self->ctor_name != NULL)
	{
	  fprintf (out, " (), new_ctor, _");
	  // TODO: lookup ctor_name, and use correct classname
	  O_CALL (token, generate);
	  fprintf (out, "_ctor_");
	  O_CALL (self->ctor_name->token, generate);
	  O_CALL (self->ctor_arguments, map_args,
		  Expression_generate_actual_argument, &is_first_arg);
	  fprintf (out, ")");
	}
      else
	{
	  fprintf (out, " (), new");
	  O_CALL (self->ctor_arguments, map_args,
		  Expression_generate_actual_argument, &is_first_arg);
	  fprintf (out, ")");
	}
    }
  else if (self->array_size)
    {
      fprintf (out, "calloc (");
      O_CALL (self->array_size, generate);
      fprintf (out, ", sizeof (");
      O_CALL (self->new_type, generate);
      fprintf (out, "))");
    }
  else
    {
      /* TODO error message */
    }
}

void
NewExpression_type_check_object (void *_item)
{
  struct CompileObject *item = O_CAST (_item, CompileObject ());
  O_CALL (item, type_check);
}

void
NewExpression_type_check_arguments (struct TokenExpression *ctor_token,
				    struct RefList *actual_arguments)
{
  if (!o_is_of (ctor_token->decl, ConstructorDeclaration ()))
    {
      error (ctor_token->token, "'%s' is not a constructor.\n",
	     ctor_token->token->name->data);
      return;
    }
  struct ConstructorDeclaration *ctor_decl =
    (struct ConstructorDeclaration *) ctor_token->decl;
  int expected_length = ctor_decl->formal_arguments->length;
  /* if last parameter is vararg, expected_length -1 */
  if (O_CALL (ctor_decl, has_var_args))
    {
	  expected_length --;
	  if (actual_arguments->length < expected_length)
	    {
	      error (ctor_token->token, "'%s' needs %d arguments, but got %d.\n",
		     ctor_token->token->name->data,
		     expected_length, actual_arguments->length);
	      return;
	    }
    }
  else
    {
      if (actual_arguments->length != expected_length)
	{
	  error (ctor_token->token, "'%s' needs %d arguments, but got %d.\n",
		 ctor_token->token->name->data,
		 expected_length, actual_arguments->length);
	  return;
	}
    }

  int i;
  for (i = 0; i < expected_length; i++)
    {
      struct ArgumentDeclaration *arg_decl =
	O_CALL (ctor_decl->formal_arguments, get, i);
      struct Expression *arg_expr = O_CALL (actual_arguments, get, i);
      O_CALL (arg_expr, type_check);
      O_CALL (arg_decl->type, type_check);
      O_CALL (arg_decl->type, assert_compatible, arg_expr->type);
    }
  for (i = expected_length; i < actual_arguments->length; i++)
    {
      struct Expression *arg_expr = O_CALL (actual_arguments, get, i);
      O_CALL (arg_expr, type_check);
    }
}

O_IMPLEMENT (NewExpression, void, type_check, (void *_self))
{
  struct NewExpression *self = O_CAST (_self, NewExpression ());
  O_CALL (self->new_type, type_check);
  if (self->ctor_arguments)
    {
      struct ObjectType *object_type = o_cast (self->new_type, ObjectType ());
      struct ClassDeclaration *class_decl =
	o_cast (object_type->decl, ClassDeclaration ());
      if (self->ctor_name)
	{
	  O_CALL (self->ctor_name, set_scope, class_decl->member_scope);
	  O_CALL (self->ctor_name, type_check);
	  NewExpression_type_check_arguments (self->ctor_name,
					      self->ctor_arguments);
	}
      else
	{
	  struct Token *ctor_token =
	    O_CALL_CLASS (Token (), new_ctor, _Token_ctor_from_token, object_type->token, "ctor", IDENTIFIER);
	  struct TokenExpression *token_expr =
	    O_CALL_CLASS (TokenExpression (), new, ctor_token);

	  O_CALL (token_expr, set_scope, class_decl->member_scope);
	  /* check whether the token "ctor" is defined */
	  if (O_CALL (token_expr->scope, exists, token_expr->token) ||
	      O_CALL (global_scope, exists, token_expr->token))
	    {
	      O_CALL (token_expr, type_check);
	      NewExpression_type_check_arguments (token_expr,
						  self->ctor_arguments);
	    }
	  else
	    {
	      /* accept empty constructor */
	      O_CALL (self->ctor_arguments, map, Declaration_list_type_check);
	    }
	}
      self->type = O_CALL (self->new_type, retain);
    }
  else if (self->array_size)
    {
      O_CALL (self->array_size, type_check);
      self->type = O_CALL_CLASS (ArrayType (), new, self->new_type);
    }
  else
    {
      /* TODO error message */
    }
}

O_IMPLEMENT (NewExpression, void, set_ctor_name,
	     (void *_self, struct TokenExpression * ctor_name))
{
  struct NewExpression *self = O_CAST (_self, NewExpression ());
  self->ctor_name = O_CALL (ctor_name, retain);
}

O_OBJECT (NewExpression, Expression);
O_OBJECT_METHOD (NewExpression, ctor);
O_OBJECT_METHOD (NewExpression, dtor);
O_OBJECT_METHOD (NewExpression, accept);
O_OBJECT_METHOD (NewExpression, set_ctor_name);
O_OBJECT_METHOD (NewExpression, generate);
O_OBJECT_METHOD (NewExpression, type_check);
O_END_OBJECT
