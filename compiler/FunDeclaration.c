#include "FunDeclaration.h"
#include "Type.h"
#include "RefList.h"
#include "Statement.h"
#include "io.h"
#include "FunctionType.h"

#define O_SUPER Declaration()

O_IMPLEMENT(FunDeclaration, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct FunDeclaration * self = O_CAST(_self, FunDeclaration());
  self = O_SUPER->ctor(self, app);
  self->return_type = o_cast(va_arg(*app, struct Type *), Type());
  O_CALL(self->return_type, retain);
  self->formal_arguments = o_cast(va_arg(*app, struct RefList *), RefList());
  O_CALL(self->formal_arguments, retain);
  self->body = O_BRANCH_CAST(va_arg(*app, struct Statement *), Statement());
  O_BRANCH_CALL(self->body, retain);
  return self;
}

O_IMPLEMENT(FunDeclaration, void *, dtor, (void *_self), (_self))
{
  struct FunDeclaration * self = O_CAST(_self, FunDeclaration());
  O_CALL(self->return_type, release);
  O_CALL(self->formal_arguments, release);
  O_BRANCH_CALL(self->body, release);
  O_BRANCH_CALL(self->type, release);
  return O_SUPER->dtor(self);
}

void FunDeclaration_generate_formal_arg(void *_decl, va_list * ap)
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

O_IMPLEMENT(FunDeclaration, void, generate, (void *_self), (_self))
{
  struct FunDeclaration * self = O_CAST(_self, FunDeclaration());
  bool first_formal_arg = true;
  O_CALL(self->return_type, generate);
  fprintf(out, " ");
  O_CALL(self->name, generate);
  fprintf(out, "(");
  O_CALL(self->formal_arguments, map_args, FunDeclaration_generate_formal_arg, &first_formal_arg);
  fprintf(out, ")\n");
  fprintf(out, "{\n");
  O_CALL(self->body, generate);
  fprintf(out, "}\n");
}

O_IMPLEMENT(FunDeclaration, void, type_check, (void *_self), (_self))
{
  struct FunDeclaration * self = O_CAST(_self, FunDeclaration());
  O_CALL(self->body, type_check);
}

O_OBJECT(FunDeclaration, Declaration);
O_OBJECT_METHOD(FunDeclaration, ctor);
O_OBJECT_METHOD(FunDeclaration, dtor);
O_OBJECT_METHOD(FunDeclaration, generate);
O_OBJECT_METHOD(FunDeclaration, type_check);
O_END_OBJECT
