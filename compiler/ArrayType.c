#include "ArrayType.h"
#include "String.h"
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

O_IMPLEMENT(ArrayType, struct String *, to_string, (void *_self), (_self))
{
  struct ArrayType *self = O_CAST(_self, ArrayType());
  struct String * string = O_CALL(self->base_type, to_string);
  O_CALL(string, append_str, "*");
  return string;
}

O_IMPLEMENT(ArrayType, struct Token *, get_token, (void *_self), (_self))
{
  struct ArrayType *self = O_CAST(_self, ArrayType());
  return O_CALL(self->base_type, get_token);
}

O_IMPLEMENT(ArrayType, bool, is_compatible, (void *_self, void *_other), (_self,_other))
{
  struct ArrayType *self = O_CAST(_self, ArrayType());
  if (O_SUPER->is_compatible(self, _other))
    {
      struct ArrayType * other = O_CAST(_other, ArrayType());
      return O_CALL(self->base_type, is_compatible, other->base_type);
    }
  return false;
}

O_OBJECT(ArrayType, Type);
O_OBJECT_METHOD(ArrayType, ctor);
O_OBJECT_METHOD(ArrayType, dtor);
O_OBJECT_METHOD(ArrayType, generate);
O_OBJECT_METHOD(ArrayType, to_string);
O_OBJECT_METHOD(ArrayType, get_token);
O_OBJECT_METHOD(ArrayType, is_compatible);
O_END_OBJECT
