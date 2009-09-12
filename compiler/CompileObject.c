#include "CompileObject.h"

#define O_SUPER RefObject()

O_IMPLEMENT(CompileObject, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct CompileObject * self = O_CAST(_self, CompileObject());
  self = O_SUPER->ctor(self, app);
  return self;
}

O_IMPLEMENT(CompileObject, void *, dtor, (void *_self), (_self))
{
  struct CompileObject *self = O_CAST(_self, CompileObject());
  return O_SUPER->dtor(self);
}

O_OBJECT(CompileObject, RefObject);
O_OBJECT_METHOD(CompileObject, ctor);
O_OBJECT_METHOD(CompileObject, dtor);
O_END_OBJECT
