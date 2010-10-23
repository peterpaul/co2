#include "FunctionDeclaration.h"
#include "Type.h"
#include "RefList.h"
#include "Statement.h"
#include "io.h"
#include "FunctionType.h"

#define O_SUPER Declaration()

static struct FunctionType * get_type(struct FunctionDeclaration * self)
{
  return o_cast(self->type, FunctionType());
}

O_IMPLEMENT(FunctionDeclaration, void *, ctor, (void *_self, va_list *app))
{
  struct FunctionDeclaration * self = O_CAST(_self, FunctionDeclaration());
  self = O_SUPER->ctor(self, app);
  self->type = o_cast(va_arg(*app, struct FunctionType *), FunctionType());
  O_CALL(self->type, retain);
  self->formal_arguments = o_cast(va_arg(*app, struct RefList *), RefList());
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
  bool first_formal_arg = true;
  O_CALL(get_type(self)->return_type, generate);
  fprintf(out, " ");
  O_CALL(self->name, generate);
  fprintf(out, "(");
  O_CALL(self->formal_arguments, map_args, FunctionDeclaration_generate_formal_arg, &first_formal_arg);
  fprintf(out, ")\n");
  fprintf(out, "{\n");
  O_CALL(self->body, generate);
  fprintf(out, "}\n");
}

O_IMPLEMENT(FunctionDeclaration, void, type_check, (void *_self))
{
  struct FunctionDeclaration * self = O_CAST(_self, FunctionDeclaration());
  O_CALL(current_context, add, self);
  O_CALL(self->body, type_check);
  O_CALL(current_context, remove_last);
}

O_OBJECT(FunctionDeclaration, Declaration);
O_OBJECT_METHOD(FunctionDeclaration, ctor);
O_OBJECT_METHOD(FunctionDeclaration, dtor);
O_OBJECT_METHOD(FunctionDeclaration, generate);
O_OBJECT_METHOD(FunctionDeclaration, type_check);
O_END_OBJECT
