#include "Type.h"

#define O_SUPER CompileObject()

O_IMPLEMENT(Type, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct Type * self = O_CAST(_self, Type());
  self = O_SUPER->ctor(self, app);
  return self;
}

O_IMPLEMENT(Type, void *, dtor, (void *_self), (_self))
{
  struct Type *self = O_CAST(_self, Type());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_OBJECT(Type, CompileObject);
O_OBJECT_METHOD(Type, ctor);
O_OBJECT_METHOD(Type, dtor);
O_END_OBJECT
