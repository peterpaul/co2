#include "VarDeclaration.h"

#define O_SUPER Declaration()

O_IMPLEMENT(VarDeclaration, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct VarDeclaration * self = O_CAST(_self, VarDeclaration());
  self = O_SUPER->ctor(self, app);
  self->type = va_arg(*app, struct Type *);
  self->variables = va_arg(*app, struct List *);
  return self;
}

O_OBJECT(VarDeclaration, Declaration);
O_OBJECT_METHOD(VarDeclaration, ctor);
O_END_OBJECT
