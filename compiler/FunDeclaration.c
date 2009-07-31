#include "FunDeclaration.h"
#include "Type.h"
#include "List.h"
#include "Statement.h"

#define O_SUPER Declaration()

O_IMPLEMENT(FunDeclaration, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct FunDeclaration * self = O_CAST(_self, FunDeclaration());
  self = O_SUPER->ctor(self, app);
  self->type = o_cast(va_arg(*app, struct Type *), Type());
  self->formal_arguments = o_cast(va_arg(*app, struct List *), List());
  self->body = o_cast(va_arg(*app, struct Statement *), Statement());
  return self;
}

O_OBJECT(FunDeclaration, Declaration);
O_OBJECT_METHOD(FunDeclaration, ctor);
O_END_OBJECT
