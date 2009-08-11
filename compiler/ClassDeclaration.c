#include "ClassDeclaration.h"
#include "Token.h"
#include "RefList.h"

#define O_SUPER Declaration()

O_IMPLEMENT(ClassDeclaration, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct ClassDeclaration * self = O_CAST(_self, ClassDeclaration());
  self = O_SUPER->ctor(self, app);
  self->superclass = o_cast(va_arg(*app, struct Type *), Token());
  self->interfaces = O_BRANCH_CAST(va_arg(*app, struct RefList *), RefList());
  self->members = o_cast(va_arg(*app, struct RefList *), RefList());
  return self;
}

O_IMPLEMENT(ClassDeclaration, void, generate, (void *_self), (_self))
{
  struct ClassDeclaration * self = O_CAST(_self, ClassDeclaration());
}

O_OBJECT(ClassDeclaration, Declaration);
O_OBJECT_METHOD(ClassDeclaration, ctor);
O_OBJECT_METHOD(ClassDeclaration, generate);
O_END_OBJECT
