#include "ObjectType.h"
#include "Token.h"
#include "ObjectTypeDeclaration.h"
#include "io.h"

#define O_SUPER Type()

O_IMPLEMENT (ObjectType, void *, ctor, (void *_self, va_list * app))
{
  struct ObjectType *self = O_CAST (_self, ObjectType ());
  self = O_SUPER->ctor (self, app);
  self->token = O_CAST (va_arg (*app, struct Token *), Token ());
  O_CALL (self->token, retain);
  self->decl =
    O_BRANCH_CAST (va_arg (*app, struct ObjectTypeDeclaration *), ObjectTypeDeclaration ());
  O_BRANCH_CALL (self->decl, retain);
  return self;
}

O_IMPLEMENT (ObjectType, void *, dtor, (void *_self))
{
  struct ObjectType *self = O_CAST (_self, ObjectType ());
  O_CALL (self->token, release);
  O_CALL (self->decl, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (ObjectType, void, type_check, (void *_self))
{
  struct ObjectType *self = O_CAST (_self, ObjectType ());
  if (self->decl == NULL)
    {
      self->decl = (struct ObjectTypeDeclaration *) O_CALL (global_scope, lookup, self->token);
      O_CAST(self->decl, ObjectTypeDeclaration ());
    }
}

O_IMPLEMENT (ObjectType, void, generate, (void *_self))
{
  struct ObjectType *self = O_CAST (_self, ObjectType ());
  fprintf (out, "struct ");
  O_CALL (self->token, generate);
  fprintf (out, "*");
}

O_IMPLEMENT (ObjectType, struct Token *, get_token, (void *_self))
{
  struct ObjectType *self = O_CAST (_self, ObjectType ());
  return self->token;
}

O_IMPLEMENT (ObjectType, struct String *, to_string, (void *_self))
{
  struct ObjectType *self = O_CAST (_self, ObjectType ());
  return O_CALL_CLASS (String (), new, "%s", self->token->name->data);;
}

O_IMPLEMENT (ObjectType, bool, is_compatible, (void *_self, void *_other))
{
  struct ObjectType *self = O_CAST (_self, ObjectType ());
  if (O_SUPER->is_compatible (self, _other))
    {
      struct ObjectType *other = O_CAST (_other, ObjectType ());
      struct Token *name_self = O_CALL (self, get_token);
      struct Token *name_other = O_CALL (other, get_token);
      return O_CALL(self->decl, is_compatible, other->decl);
    }
  return false;
}

O_OBJECT (ObjectType, Type);
O_OBJECT_METHOD (ObjectType, ctor);
O_OBJECT_METHOD (ObjectType, dtor);
O_OBJECT_METHOD (ObjectType, generate);
O_OBJECT_METHOD (ObjectType, type_check);
O_OBJECT_METHOD (ObjectType, get_token);
O_OBJECT_METHOD (ObjectType, to_string);
O_OBJECT_METHOD (ObjectType, is_compatible);
O_END_OBJECT
