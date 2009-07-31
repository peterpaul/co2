#include "ClassDeclaration.h"
#include "Token.h"
#include "List.h"

#define O_SUPER Declaration()

O_IMPLEMENT(ClassDeclaration, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct ClassDeclaration * self = O_CAST(_self, ClassDeclaration());
  self = O_SUPER->ctor(self, app);
  self->name = o_cast(va_arg(*app, struct Type *), Token());
  self->superclass = o_cast(va_arg(*app, struct Type *), Token());
  self->interfaces = O_BRANCH_CAST(va_arg(*app, struct List *), List());
  self->members = o_cast(va_arg(*app, struct List *), List());
  return self;
}

O_OBJECT(ClassDeclaration, Declaration);
O_OBJECT_METHOD(ClassDeclaration, ctor);
O_END_OBJECT
