#include "InterfaceDeclaration.h"

#define O_SUPER Declaration()

O_IMPLEMENT(InterfaceDeclaration, void *, ctor, (void *_self, va_list *app))
{
  struct InterfaceDeclaration * self = O_CAST(_self, InterfaceDeclaration());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT(InterfaceDeclaration, void *, dtor, (void *_self))
{
  struct InterfaceDeclaration *self = O_CAST(_self, InterfaceDeclaration());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_OBJECT(InterfaceDeclaration, Declaration);
O_OBJECT_METHOD(InterfaceDeclaration, ctor);
O_OBJECT_METHOD(InterfaceDeclaration, dtor);
O_END_OBJECT
