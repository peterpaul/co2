#include "Type.h"
#include "ArrayType.h"
#include "PrimitiveType.h"
#include "Token.h"

#define O_SUPER CompileObject()

O_IMPLEMENT(Type, void *, ctor, (void *_self, va_list *app))
{
  struct Type * self = O_CAST(_self, Type());
  self = O_SUPER->ctor(self, app);
  return self;
}

O_IMPLEMENT(Type, void *, dtor, (void *_self))
{
  struct Type *self = O_CAST(_self, Type());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(Type, bool, is_compatible, (void *_self, void *_other))
{
  struct Type *self = O_CAST(_self, Type());
  if (_other)
    {
      struct Type *other = O_CAST(_other, Type());
      return o_is_a(other, self->class);
    }
  else
    {
      return false;
    }
}

O_IMPLEMENT(Type, void, assert_compatible, (void *_self, void *_other))
{
  struct Type *self = O_CAST(_self, Type());
  if (!O_CALL(self, is_compatible, _other))
    {
      if (_other)
	{
	  struct Type * other = O_CAST(_other, Type());
	  struct Token * token = O_CALL(self, get_token);
	  struct String * self_string = O_CALL(self, to_string);
	  struct String * other_string = O_CALL(other, to_string);
	  error(token, "incompatible types: %s and %s\n", self_string->data, other_string->data);
	  O_CALL(self_string, delete);
	  O_CALL(other_string, delete);
	}
      else
	{
	  struct Token * token = O_CALL(self, get_token);
	  struct String * self_string = O_CALL(self, to_string);
	  error(token, "incompatible types: %s and %s\n", self_string->data, NULL);
	  O_CALL(self_string, delete);
	}
    }
}

O_IMPLEMENT(Type, void, type_check, (void *_self))
{
  struct Type * self = O_CAST(_self, Type());
}

O_OBJECT(Type, CompileObject);
O_OBJECT_METHOD(Type, ctor);
O_OBJECT_METHOD(Type, dtor);
O_OBJECT_METHOD(Type, is_compatible);
O_OBJECT_METHOD(Type, assert_compatible);
O_OBJECT_METHOD(Type, type_check);
O_END_OBJECT
