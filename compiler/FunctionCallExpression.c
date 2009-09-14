#include "FunctionCallExpression.h"
#include "BinaryExpression.h"
#include "FunDeclaration.h"
#include "ArgDeclaration.h"
#include "Token.h"
#include "TokenExpression.h"
#include "Type.h"
#include "RefList.h"
#include "io.h"

#define O_SUPER Expression()

O_IMPLEMENT(FunctionCallExpression, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct FunctionCallExpression * self = O_CAST(_self, FunctionCallExpression());
  self = O_SUPER->ctor(self, app);
  self->function = O_CAST(va_arg(*app, struct Expression *), Expression());
  O_CALL(self->function, retain);
  self->actual_arguments = O_CAST(va_arg(*app, struct RefList *), RefList());
  O_CALL(self->actual_arguments, retain);
  return self;
}

O_IMPLEMENT(FunctionCallExpression, void *, dtor, (void *_self), (_self))
{
  struct FunctionCallExpression *self = O_CAST(_self, FunctionCallExpression());
  O_CALL(self->function, release);
  O_CALL(self->actual_arguments, release);
  return O_SUPER->dtor(self);
}

void FunctionCallExpression_generate_actual_arguments(void *_arg, va_list *app)
{
  struct Expression *arg = O_CAST(_arg, Expression());
  bool * is_first_arg = va_arg(*app, bool *);
  if (!*is_first_arg) 
    {
      fprintf(out, ", ");
    }
  else
    {
      *is_first_arg = false;
    }
  O_CALL(arg, generate);
}

O_IMPLEMENT(FunctionCallExpression, void, generate, (void *_self), (_self))
{
  struct FunctionCallExpression *self = O_CAST(_self, FunctionCallExpression());
  bool is_first_arg = true;
  O_CALL(self->function, generate);
  fprintf(out, "(");
  if (o_is_of(self->function, BinaryExpression()))
    {
      struct BinaryExpression * function = (struct BinaryExpression *)self->function;
      if (function->operator->type == '.') 
	{
	  O_CALL(function->operand[0], generate);
	  is_first_arg = false;
	}
    }
  O_CALL(self->actual_arguments, map_args, FunctionCallExpression_generate_actual_arguments, &is_first_arg);
  fprintf(out, ")");
}

O_IMPLEMENT(FunctionCallExpression, void, type_check, (void *_self), (_self))
{
  struct FunctionCallExpression *self = O_CAST(_self, FunctionCallExpression());
  if (o_is_a(self->function, TokenExpression()))
    {
      struct TokenExpression * function = (struct TokenExpression *) self->function;
      O_CALL(self->function, type_check);
      self->type = O_CALL(self->function->type, retain);
      if (!o_is_of(function->decl, FunDeclaration()))
	{
	  error(function->token, "%s is not a function.\n", function->token->name->data);
	}
      else
	{
	  struct FunDeclaration * fun_decl = (struct FunDeclaration *) function->decl;
	  if (self->actual_arguments->length < fun_decl->formal_arguments->length)
	    {
	      error(function->token, "%s needs %d arguments, but got %d.\n", function->token->name->data, fun_decl->formal_arguments->length, self->actual_arguments->length);
	      return;
	    }
	  int i;
	  for (i = 0; i < fun_decl->formal_arguments->length; i++)
	    {
	      struct ArgDeclaration * arg_decl = O_CALL(fun_decl->formal_arguments, get, i);
	      struct Expression * arg_expr = O_CALL(self->actual_arguments, get, i);
	      O_CALL(arg_expr, type_check);
	      O_CALL(arg_decl->type, assert_compatible, arg_expr->type);
	    }
	}
    }
}

O_OBJECT(FunctionCallExpression, Expression);
O_OBJECT_METHOD(FunctionCallExpression, ctor);
O_OBJECT_METHOD(FunctionCallExpression, dtor);
O_OBJECT_METHOD(FunctionCallExpression, generate);
O_OBJECT_METHOD(FunctionCallExpression, type_check);
O_END_OBJECT
