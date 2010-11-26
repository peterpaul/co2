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

static struct FunctionType * get_type(struct FunctionDeclaration * self)
{
  return o_cast(self->type, FunctionType());
}

O_IMPLEMENT(FunctionDeclaration, void *, ctor, (void *_self, va_list *app))
{
  struct FunctionDeclaration * self = O_CAST(_self, FunctionDeclaration());
  self = O_SUPER->ctor(self, app);
  self->type = O_CAST(va_arg(*app, struct FunctionType *), FunctionType());
  O_CALL(self->type, retain);
  self->formal_arguments = O_CAST(va_arg(*app, struct RefList *), RefList());
  O_CALL(self->formal_arguments, retain);
  self->body = O_BRANCH_CAST(va_arg(*app, struct Statement *), Statement());
  O_BRANCH_CALL(self->body, retain);
  return self;
}

O_IMPLEMENT(FunctionDeclaration, void *, dtor, (void *_self))
{
  struct FunctionDeclaration * self = O_CAST(_self, FunctionDeclaration());
  O_CALL(self->formal_arguments, release);
  O_BRANCH_CALL(self->body, release);
  return O_SUPER->dtor(self);
}

void FunctionDeclaration_generate_formal_arg(void *_decl, va_list * ap)
{
  struct Declaration *decl = O_CAST(_decl, Declaration());
  bool *first_formal_arg = va_arg(*ap, bool *);
  if (*first_formal_arg == false) 
    {
      fprintf(out, ", ");
    }
  *first_formal_arg = false;
  O_CALL(decl, generate);
}

O_IMPLEMENT(FunctionDeclaration, void, generate, (void *_self))
{
  struct FunctionDeclaration * self = O_CAST(_self, FunctionDeclaration());
  // don't generate if external definition
  if (self->include_file) 
    {
      fprintf(out, "#include ");
      O_CALL(self->include_file, generate);
      fprintf(out, "\n");
      return;
    }
  bool first_formal_arg = true;
  struct FunctionType * function_type = get_type(self);
  O_CALL(function_type->return_type, generate);
  fprintf(out, " ");
  O_CALL(self->name, generate);
  fprintf(out, "(");
  O_CALL(self->formal_arguments, map_args, FunctionDeclaration_generate_formal_arg, &first_formal_arg);
  fprintf(out, ")\n");
  fprintf(out, "{\n");

  if (!o_is_of(function_type->return_type, PrimitiveType()) ||
      ((struct PrimitiveType *)(function_type->return_type))->token->type != VOID)
    {
      O_CALL(function_type->return_type, generate);
      fprintf(out, " return_value;\n");
    }

  if (function_type->has_var_args)
    {
      fprintf(out, "va_list ap;\n");
      fprintf(out, "va_start(ap, ");
      struct ArgumentDeclaration * arg_decl = O_CALL(self->formal_arguments, get, self->formal_arguments->length - 2);
      O_CALL(arg_decl->name, generate);
      fprintf(out, ");\n");
    }
  O_BRANCH_CALL(self->body, generate);

  if (!o_is_of(function_type->return_type, PrimitiveType()) ||
      ((struct PrimitiveType *)(function_type->return_type))->token->type != VOID)
    {
      fprintf(out, "function_end:\n");
    }

  if (function_type->has_var_args)
    {
      fprintf(out, "va_end(ap);\n");
    }

  if (!o_is_of(function_type->return_type, PrimitiveType()) ||
      ((struct PrimitiveType *)(function_type->return_type))->token->type != VOID)
    {
      fprintf(out, "return return_value;\n");
    }

  fprintf(out, "}\n\n");
}

static void FunctionDeclaration_find_in_interface(void *_self, va_list *app)
{
  struct Token * self = O_CAST(_self, Token());
  struct FunctionDeclaration * function_decl = O_CAST(va_arg(*app, struct FunctionDeclaration *), FunctionDeclaration());
  if (O_CALL(global_scope, exists_in_this_scope, self))
    {
      struct Declaration * _decl = O_CALL(global_scope, lookup_in_this_scope, self);
      if (o_is_of(_decl, InterfaceDeclaration())) 
	{
	  function_decl->interface_decl = O_CAST(_decl, InterfaceDeclaration());
	}
      else
	{
	  error(function_decl->name, "%s is not an interface", _decl->name->name->data);
	}
    }
}

O_IMPLEMENT(FunctionDeclaration, void, type_check, (void *_self))
{
  struct FunctionDeclaration * self = O_CAST(_self, FunctionDeclaration());
  O_CALL(current_context, add, self);
  O_CALL(self->type, type_check);
  O_BRANCH_CALL(self->body, type_check);

  struct FunctionType * function_type = get_type(self);
  struct ClassDeclaration * class_decl = O_CALL(current_context, find, ClassDeclaration());
  if (function_type->has_var_args)
    {
      if (class_decl == NULL &&
	  self->formal_arguments->length <= 1)
	{
	  error(self->name, "variable argument list only supported on functions with at least one fixed argument.\n");
	}
    }
  if (class_decl && class_decl->interfaces)
    {
      O_CALL(class_decl->interfaces, map_args, FunctionDeclaration_find_in_interface, self);
    }
  O_CALL(current_context, remove_last);
}

O_OBJECT(FunctionDeclaration, Declaration);
O_OBJECT_METHOD(FunctionDeclaration, ctor);
O_OBJECT_METHOD(FunctionDeclaration, dtor);
O_OBJECT_METHOD(FunctionDeclaration, generate);
O_OBJECT_METHOD(FunctionDeclaration, type_check);
O_END_OBJECT
