#include "Declaration.h"
#include "Token.h"
#include "Type.h"
#include "ClassDeclaration.h"

#define O_SUPER CompileObject()

void
Declaration_list_set_include_header (void *_decl, va_list * app)
{
  struct Declaration *decl = o_cast (_decl, Declaration ());
  struct Token *header_file = O_GET_ARG (Token);
  O_CALL (decl, set_include_file, header_file);
}

void
Declaration_list_type_check (void *_decl)
{
  struct Declaration *decl = O_CAST (_decl, Declaration ());
  O_CALL (decl, type_check);
}

int
Declaration_new_member_filter (void *_member, va_list * app)
{
  struct Declaration *member = O_CAST (_member, Declaration ());
  struct Class *_type = va_arg (*app, struct Class *);
  struct Class *type = O_IS_CLASS (_type);
  return O_BRANCH_CALL (member->scope->parent, lookup_type_in_this_scope,
			member->name, type) == NULL;
}

O_IMPLEMENT (Declaration, void *, ctor, (void *_self, va_list * app))
{
  struct Declaration *self = O_CAST (_self, Declaration ());
  self = O_SUPER->ctor (self, app);
  self->name = O_RETAIN_ARG (Token);
  return self;
}

O_IMPLEMENT (Declaration, void *, dtor, (void *_self))
{
  struct Declaration *self = O_CAST (_self, Declaration ());
  O_CALL (self->name, release);
  O_BRANCH_CALL (self->type, release);
  O_BRANCH_CALL (self->include_file, release);
  O_BRANCH_CALL (self->class_decl, release);
  return O_SUPER->dtor (self);
}

O_IMPLEMENT (Declaration, void, set_scope, (void *_self, void *_scope))
{
  struct Declaration *self = O_CAST (_self, Declaration ());
  self->scope = o_cast (_scope, Scope ());
}

O_IMPLEMENT (Declaration, void, set_class_decl,
	     (void *_self, void *_class_decl))
{
  struct Declaration *self = O_CAST (_self, Declaration ());
  self->class_decl = o_cast (_class_decl, ClassDeclaration ());
  O_CALL (self->class_decl, retain);
}

O_IMPLEMENT (Declaration, void, set_include_file,
	     (void *_self, void *_include_file))
{
  struct Declaration *self = O_CAST (_self, Declaration ());
  self->include_file = o_cast (_include_file, Token ());
  O_CALL (self->include_file, retain);
}

O_OBJECT (Declaration, CompileObject);
O_OBJECT_METHOD (Declaration, ctor);
O_OBJECT_METHOD (Declaration, dtor);
O_OBJECT_METHOD (Declaration, set_scope);
O_OBJECT_METHOD (Declaration, set_class_decl);
O_OBJECT_METHOD (Declaration, set_include_file);
O_END_OBJECT
