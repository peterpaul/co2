#include "co2/TypeDeclaration.h"
#include "co2/PrimitiveType.h"
#include "co2/ObjectType.h"
#include "co2/Type.h"
#include "co2/io.h"

#define O_SUPER ObjectTypeDeclaration()

O_IMPLEMENT(TypeDeclaration, void *, ctor, (void *_self, va_list *app))
{
  struct TypeDeclaration * self = O_CAST(_self, TypeDeclaration());
  self = O_SUPER->ctor(self, app);
  self->type = O_RETAIN_ARG (Type);
  self->is_struct = va_arg (*app, bool);
  return self;
}

O_IMPLEMENT(TypeDeclaration, void *, dtor, (void *_self))
{
  struct TypeDeclaration *self = O_CAST(_self, TypeDeclaration());
  O_CALL (self->type, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT (TypeDeclaration, void, accept, (void *_self, struct BaseCompileObjectVisitor *visitor))
{
  struct TypeDeclaration *self = O_CAST (_self, TypeDeclaration ());
  O_CALL (self->type, accept, visitor);
  O_CALL (visitor, visit, self);
}

O_IMPLEMENT(TypeDeclaration, void, type_check, (void *_self))
{
  struct TypeDeclaration *self = O_CAST(_self, TypeDeclaration());
  O_CALL (self->type, type_check);
  if (self->is_struct)
    {
      struct PrimitiveType * type = O_CAST (self->type, PrimitiveType ());
      struct Declaration * decl = O_CALL (global_scope, lookup, type->token);
      if (decl)
	{
	  self->type = O_CALL_CLASS (ObjectType (), new, type->token, decl);
	  O_CALL (self->type, retain);
	  O_CALL (self->type, type_check);
	  O_CALL (type, release);
	}
    }
}

O_IMPLEMENT (TypeDeclaration, bool, is_compatible,
	     (void *_self, void *_other))
{
  struct TypeDeclaration *self = O_CAST (_self, TypeDeclaration ());
  /* TODO: do stuff? */
  return true;
}

O_OBJECT(TypeDeclaration, ObjectTypeDeclaration);
O_OBJECT_METHOD(TypeDeclaration, ctor);
O_OBJECT_METHOD(TypeDeclaration, dtor);
O_OBJECT_METHOD(TypeDeclaration, accept);
O_OBJECT_METHOD(TypeDeclaration, type_check);
O_OBJECT_METHOD(TypeDeclaration, is_compatible);
O_END_OBJECT