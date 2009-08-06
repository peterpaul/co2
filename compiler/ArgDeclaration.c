#include "ArgDeclaration.h"

#define O_SUPER Declaration()

O_IMPLEMENT(ArgDeclaration, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct ArgDeclaration * self = O_CAST(_self, ArgDeclaration());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT(ArgDeclaration, void *, dtor, (void *_self), (_self))
{
  struct ArgDeclaration *self = O_CAST(_self, ArgDeclaration());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_OBJECT(ArgDeclaration, Declaration);
O_OBJECT_METHOD(ArgDeclaration, ctor);
O_OBJECT_METHOD(ArgDeclaration, dtor);
O_END_OBJECT
