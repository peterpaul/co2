#include "co2/ImplA.h"

#define O_SUPER Object()

O_IMPLEMENT(ImplA, void *, ctor, (void *_self, va_list *app))
{
  struct ImplA * self = O_CAST(_self, ImplA());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  return self;
}

O_IMPLEMENT(ImplA, void *, dtor, (void *_self))
{
  struct ImplA *self = O_CAST(_self, ImplA());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_IMPLEMENT_IF(ImplA, int, getValue, (void *_self), (_self))
{
  struct ImplA *self = O_CAST(_self, ImplA());
  return self->value;
}

O_IMPLEMENT_IF (ImplA, void, setValue, (void *_self, int _value), (_self,_value))
{
  struct ImplA *self = O_CAST(_self, ImplA());
  self->value = _value;
}

O_OBJECT(ImplA, Object);
O_OBJECT_METHOD(ImplA, ctor);
O_OBJECT_METHOD(ImplA, dtor);
O_OBJECT_METHOD(ImplA, getValue);
O_OBJECT_METHOD(ImplA, setValue);
O_OBJECT_IF(IfA);
O_OBJECT_IF_METHOD(ImplA, getValue);
O_OBJECT_IF_METHOD(ImplA, setValue);
O_OBJECT_IF_END
O_END_OBJECT
