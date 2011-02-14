#include "co2/Type.h"
#include "co2/ArrayType.h"
#include "co2/PrimitiveType.h"
#include "co2/ObjectType.h"
#include "co2/TypeDeclaration.h"
#include "co2/Token.h"
#include "grammar.h"

#define O_SUPER CompileObject()

O_IMPLEMENT (Type, void *, ctor, (void *_self, va_list * app))
{
  struct Type *self = O_CAST (_self, Type ());
  self = O_SUPER->ctor (self, app);
  return self;
}

O_IMPLEMENT (Type, void *, dtor, (void *_self))
{
  struct Type *self = O_CAST (_self, Type ());
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (Type, struct Type *, get_declared_type, (void *_self))
{
  struct Type *self = O_CAST (_self, Type ());
  if (o_is_of (self, ObjectType ()))
    {
      struct ObjectType * o_type = O_CAST (self, ObjectType ());
      if (o_type->decl && o_is_of (o_type->decl, TypeDeclaration ()))
	{
	  struct TypeDeclaration * o_type_decl = O_CAST (o_type->decl, TypeDeclaration ());
	  return o_type_decl->type;
	}
    }
  return self;
}

O_IMPLEMENT (Type, bool, is_compatible, (void *_self, void *_other))
{
  struct Type *self = O_CAST (_self, Type ());
  if (_other)
    {
      struct Type *other = O_CAST (_other, Type ());
      other = O_CALL (other, get_declared_type);
      return o_is_a (other, O_CALL (self, get_declared_type)->class);
    }
  else
    {
      return false;
    }
}

O_IMPLEMENT (Type, void, assert_compatible, (void *_self, void *_other))
{
  struct Type *self = O_CAST (_self, Type ());
  if (!O_CALL (self, is_compatible, _other))
    {
      if (_other)
	{
	  struct Type *other = O_CAST (_other, Type ());
	  struct Token *token = O_CALL (self, get_token);
	  struct String *self_string = O_CALL (self, to_string);
	  struct String *other_string = O_CALL (other, to_string);
	  error (token, "incompatible types: %s and %s\n", self_string->data,
		 other_string->data);
	  O_CALL (self_string, delete);
	  O_CALL (other_string, delete);
	}
      else
	{
	  struct Token *token = O_CALL (self, get_token);
	  struct String *self_string = O_CALL (self, to_string);
	  error (token, "incompatible types: %s and %s\n", self_string->data,
		 NULL);
	  O_CALL (self_string, delete);
	}
    }
}

O_IMPLEMENT (Type, void, assume_compatible, (void *_self, void *_other))
{
  struct Type *self = O_CAST (_self, Type ());
  if (!O_CALL (self, is_compatible, _other))
    {
      if (_other)
	{
	  struct Type *other = O_CAST (_other, Type ());
	  struct Token *token = O_CALL (self, get_token);
	  struct String *self_string = O_CALL (self, to_string);
	  struct String *other_string = O_CALL (other, to_string);
	  warning (token, "incompatible types: %s and %s\n", self_string->data,
		 other_string->data);
	  O_CALL (self_string, delete);
	  O_CALL (other_string, delete);
	}
      else
	{
	  struct Token *token = O_CALL (self, get_token);
	  struct String *self_string = O_CALL (self, to_string);
	  warning (token, "incompatible types: %s and %s\n", self_string->data,
		 NULL);
	  O_CALL (self_string, delete);
	}
    }
}

O_IMPLEMENT (Type, void, type_check, (void *_self))
{
  struct Type *self = O_CAST (_self, Type ());
}

O_IMPLEMENT (Type, bool, is_void_ptr, (void *_self))
{
  struct Type *self = O_CAST (_self, Type ());
  if (o_is_of (self, ArrayType ()))
    {
      struct ArrayType *array_type = O_CAST (self, ArrayType ());
      if (o_is_of (array_type->base_type, PrimitiveType ()))
	{
	  struct PrimitiveType *primitive_type = O_CAST (array_type->base_type, PrimitiveType ());
	  if (primitive_type->token->type == VOID)
	    {
	      return true;
	    }
	}
    }
  return false;
}

O_OBJECT (Type, CompileObject);
O_OBJECT_METHOD (Type, ctor);
O_OBJECT_METHOD (Type, dtor);
O_OBJECT_METHOD (Type, is_compatible);
O_OBJECT_METHOD (Type, assert_compatible);
O_OBJECT_METHOD (Type, assume_compatible);
O_OBJECT_METHOD (Type, type_check);
O_OBJECT_METHOD (Type, is_void_ptr);
O_OBJECT_METHOD (Type, get_declared_type);
O_END_OBJECT
