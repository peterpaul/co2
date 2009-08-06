#include "PrimitiveType.h"

#define O_SUPER Type()

O_IMPLEMENT(PrimitiveType, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct PrimitiveType * self = O_CAST(_self, PrimitiveType());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT(PrimitiveType, void *, dtor, (void *_self), (_self))
{
  struct PrimitiveType *self = O_CAST(_self, PrimitiveType());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_OBJECT(PrimitiveType, Type);
O_OBJECT_METHOD(PrimitiveType, ctor);
O_OBJECT_METHOD(PrimitiveType, dtor);
O_END_OBJECT
