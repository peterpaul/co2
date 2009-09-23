#include "ArgDeclaration.h"
#include "Type.h"
#include "io.h"

#define O_SUPER Declaration()

O_IMPLEMENT(ArgDeclaration, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct ArgDeclaration * self = O_CAST(_self, ArgDeclaration());
  self = O_SUPER->ctor(self, app);
  self->type = O_CAST(va_arg(*app, struct Type *), Type());
  O_CALL(self->type, retain);
  return self;
}

O_IMPLEMENT(ArgDeclaration, void *, dtor, (void *_self), (_self))
{
  struct ArgDeclaration *self = O_CAST(_self, ArgDeclaration());
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(ArgDeclaration, void, generate, (void *_self), (_self))
{
  struct ArgDeclaration *self = O_CAST(_self, ArgDeclaration());
  O_CALL(self->type, generate);
  fprintf(out, " ");
  O_CALL(self->name, generate);
}

O_OBJECT(ArgDeclaration, Declaration);
O_OBJECT_METHOD(ArgDeclaration, ctor);
O_OBJECT_METHOD(ArgDeclaration, dtor);
O_OBJECT_METHOD(ArgDeclaration, generate);
O_END_OBJECT
