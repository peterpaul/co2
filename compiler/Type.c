#include "Type.h"
#include "ArrayType.h"
#include "PrimitiveType.h"
#include "Token.h"

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

O_IMPLEMENT(Type, bool, is_compatible, (void *_self, void *_other), (_self,_other))
{
  struct Type *self = O_CAST(_self, Type());
  struct Type *other = O_CAST(_other, Type());
  if (o_is_a(other, self->class))
    {
      if (o_is_a(other, ArrayType()))
	{
	  struct ArrayType * array_self = (struct ArrayType *)self;
	  struct ArrayType * array_other = (struct ArrayType *)other;
	  return O_CALL(array_self->base_type, is_compatible, array_other->base_type);
	}
      else
	{
	  assertTrue(o_is_a(self, PrimitiveType()), "Not a primitive type!");
	  struct Token * name_self = O_CALL(self, get_token);
	  struct Token * name_other = O_CALL(other, get_token);
	  return O_CALL(name_self->name, compare, name_other->name) == 0;
	}
    }
  
  return false;
}

O_IMPLEMENT(Type, void, assert_compatible, (void *_self, void *_other), (_self, _other))
{
  struct Type *self = O_CAST(_self, Type());
  struct Type *other = O_CAST(_other, Type());
  if (!O_CALL(self, is_compatible, other))
    {
      struct Token * token = O_CALL(self, get_token);
      struct String * self_string = O_CALL(self, to_string);
      struct String * other_string = O_CALL(other, to_string);
      error(token, "incompatible types: %s and %s\n", self_string->data, other_string->data);
      O_CALL(self_string, delete);
      O_CALL(other_string, delete);
    }
}

O_OBJECT(Type, CompileObject);
O_OBJECT_METHOD(Type, ctor);
O_OBJECT_METHOD(Type, dtor);
O_OBJECT_METHOD(Type, is_compatible);
O_OBJECT_METHOD(Type, assert_compatible);
O_END_OBJECT
