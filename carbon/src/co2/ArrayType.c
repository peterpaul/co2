#include "co2/ArrayType.h"
#include "co2/String.h"
#include "co2/io.h"

#define O_SUPER Type()

O_IMPLEMENT (ArrayType, void *, ctor, (void *_self, va_list * app))
{
  struct ArrayType *self = O_CAST (_self, ArrayType ());
  self = O_SUPER->ctor (self, app);
  self->base_type = O_RETAIN_ARG (Type);
  return self;
}

O_IMPLEMENT (ArrayType, void *, dtor, (void *_self))
{
  struct ArrayType *self = O_CAST (_self, ArrayType ());
  O_CALL (self->base_type, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (ArrayType, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct ArrayType *self = O_CAST (_self, ArrayType ());
  O_CALL (self->base_type, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT (ArrayType, void, type_check, (void *_self))
{
  struct ArrayType *self = O_CAST (_self, ArrayType ());
  O_CALL (self->base_type, type_check);
}

O_IMPLEMENT (ArrayType, void, generate, (void *_self))
{
  struct ArrayType *self = O_CAST (_self, ArrayType ());
  O_CALL (self->base_type, generate);
  fprintf (out, "*");
}

O_IMPLEMENT (ArrayType, struct String *, to_string, (void *_self))
{
  struct ArrayType *self = O_CAST (_self, ArrayType ());
  struct String *string = O_CALL (self->base_type, to_string);
  O_CALL (string, append_str, "*");
  return string;
}

O_IMPLEMENT (ArrayType, struct Token *, get_token, (void *_self))
{
  struct ArrayType *self = O_CAST (_self, ArrayType ());
  return O_CALL (self->base_type, get_token);
}

O_IMPLEMENT (ArrayType, bool, is_compatible, (void *_self, void *_other))
{
  struct ArrayType *self = O_CAST (_self, ArrayType ());
  if (O_SUPER->is_compatible (self, _other))
    {
      struct ArrayType *other = O_CAST (_other, ArrayType ());
      if (O_CALL (self, is_void_ptr) || O_CALL (other, is_void_ptr))
	{
	  return true;
	}
      return O_CALL (self->base_type, is_compatible, other->base_type);
    }
  return false;
}

O_OBJECT (ArrayType, Type);
O_OBJECT_METHOD (ArrayType, ctor);
O_OBJECT_METHOD (ArrayType, dtor);
O_OBJECT_METHOD (ArrayType, accept);
O_OBJECT_METHOD (ArrayType, type_check);
O_OBJECT_METHOD (ArrayType, generate);
O_OBJECT_METHOD (ArrayType, to_string);
O_OBJECT_METHOD (ArrayType, get_token);
O_OBJECT_METHOD (ArrayType, is_compatible);
O_END_OBJECT
