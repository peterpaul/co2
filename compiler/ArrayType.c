#include "ArrayType.h"
#include "io.h"

#define O_SUPER Type()

O_IMPLEMENT(ArrayType, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct ArrayType * self = O_CAST(_self, ArrayType());
  self = O_SUPER->ctor(self, app);
  self->base_type = O_CALL(va_arg(*app, struct Type *), retain);
  return self;
}

O_IMPLEMENT(ArrayType, void *, dtor, (void *_self), (_self))
{
  struct ArrayType *self = O_CAST(_self, ArrayType());
  O_CALL(self->base_type, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(ArrayType, void, generate, (void *_self), (_self))
{
  struct ArrayType *self = O_CAST(_self, ArrayType());
  O_CALL(self->base_type, generate);
  fprintf(out, "*");
}

O_OBJECT(ArrayType, Type);
O_OBJECT_METHOD(ArrayType, ctor);
O_OBJECT_METHOD(ArrayType, dtor);
O_OBJECT_METHOD(ArrayType, generate);
O_END_OBJECT
