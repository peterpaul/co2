#include "StructDeclaration.h"
#include "Scope.h"
#include "io.h"

#define O_SUPER ObjectTypeDeclaration()

O_IMPLEMENT(StructDeclaration, void *, ctor, (void *_self, va_list *app))
{
  struct StructDeclaration * self = O_CAST(_self, StructDeclaration());
  self = O_SUPER->ctor(self, app);
  self->member_scope = O_GET_ARG (Scope);
  self->members = O_RETAIN_ARG (RefList);
  return self;
}

O_IMPLEMENT(StructDeclaration, void *, dtor, (void *_self))
{
  struct StructDeclaration *self = O_CAST(_self, StructDeclaration());
  return O_SUPER->dtor(self);
}

O_IMPLEMENT (StructDeclaration, void, type_check, (void *_self))
{
  struct StructDeclaration *self = O_CAST (_self, StructDeclaration ());
  O_CALL (current_context, add, self);
  O_CALL (self->members, map, Declaration_list_type_check);
  O_CALL (current_context, remove_last);
}

O_IMPLEMENT (StructDeclaration, bool, is_compatible,
	     (void *_self, void *_other))
{
  struct StructDeclaration *self = O_CAST (_self, StructDeclaration ());
  struct StructDeclaration *other = o_cast (_other, StructDeclaration ());
  return self == other;
}

static void Declaration_list_generate (void *_self)
{
  struct Declaration * self = O_CAST (_self, Declaration ());
  O_CALL (self, generate);
}

O_IMPLEMENT (StructDeclaration, void, generate, (void *_self))
{
  struct StructDeclaration *self = O_CAST (_self, StructDeclaration ());
  fprintf (out, "struct ");
  O_CALL (self->name, generate);
  fprintf (out, " {\n");
  O_CALL (self->members, map, Declaration_list_generate);
  fprintf (out, "};\n\n");
}

O_OBJECT(StructDeclaration, ObjectTypeDeclaration);
O_OBJECT_METHOD(StructDeclaration, ctor);
O_OBJECT_METHOD(StructDeclaration, dtor);
O_OBJECT_METHOD(StructDeclaration, type_check);
O_OBJECT_METHOD(StructDeclaration, is_compatible);
O_OBJECT_METHOD(StructDeclaration, generate);
O_END_OBJECT
