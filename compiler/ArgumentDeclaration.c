#include "ArgumentDeclaration.h"
#include "Type.h"
#include "io.h"

#define O_SUPER Declaration()

O_IMPLEMENT(ArgumentDeclaration, void *, ctor, (void *_self, va_list *app))
{
  struct ArgumentDeclaration * self = O_CAST(_self, ArgumentDeclaration());
  self = O_SUPER->ctor(self, app);
  self->type = O_CAST(va_arg(*app, struct Type *), Type());
  O_CALL(self->type, retain);
  return self;
}

O_IMPLEMENT(ArgumentDeclaration, void *, dtor, (void *_self))
{
  struct ArgumentDeclaration *self = O_CAST(_self, ArgumentDeclaration());
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(ArgumentDeclaration, void, generate, (void *_self))
{
  struct ArgumentDeclaration *self = O_CAST(_self, ArgumentDeclaration());
  O_CALL(self->type, generate);
  fprintf(out, " ");
  O_CALL(self->name, generate);
}

O_OBJECT(ArgumentDeclaration, Declaration);
O_OBJECT_METHOD(ArgumentDeclaration, ctor);
O_OBJECT_METHOD(ArgumentDeclaration, dtor);
O_OBJECT_METHOD(ArgumentDeclaration, generate);
O_END_OBJECT
