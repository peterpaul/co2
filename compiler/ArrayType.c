#include "ArrayType.h"

#define O_SUPER Type()

O_IMPLEMENT(ArrayType, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct ArrayType * self = O_CAST(_self, ArrayType());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT(ArrayType, void *, dtor, (void *_self), (_self))
{
  struct ArrayType *self = O_CAST(_self, ArrayType());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_OBJECT(ArrayType, Type);
O_OBJECT_METHOD(ArrayType, ctor);
O_OBJECT_METHOD(ArrayType, dtor);
O_END_OBJECT
