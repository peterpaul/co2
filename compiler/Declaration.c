#include "Declaration.h"
#include "Token.h"
#include "Type.h"
#include "ClassDeclaration.h"

#define O_SUPER CompileObject()

void Declaration_list_set_include_header(void * _decl, va_list * app)
{
  struct Declaration * decl = o_cast(_decl, Declaration());
  struct Token * header_file = o_cast(va_arg(*app, struct Token *), Token());
  O_CALL(decl, set_include_file, header_file);
}

O_IMPLEMENT(Declaration, void *, ctor, (void *_self, va_list *app))
{
  struct Declaration * self = O_CAST(_self, Declaration());
  self = O_SUPER->ctor(self, app);  
  self->name = o_cast(va_arg(*app, struct Token *), Token());
  O_CALL(self->name, retain);
  return self;
}

O_IMPLEMENT(Declaration, void *, dtor, (void *_self))
{
  struct Declaration *self = O_CAST(_self, Declaration());
  O_CALL(self->name, release);
  O_BRANCH_CALL(self->type, release);
  O_BRANCH_CALL(self->include_file, release);
  O_BRANCH_CALL(self->class_decl, release);
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(Declaration, void, set_scope, (void *_self, void *_scope))
{
  struct Declaration *self = O_CAST(_self, Declaration());
  self->scope = o_cast(_scope, Scope());
}

O_IMPLEMENT(Declaration, void, set_class_decl, (void *_self, void *_class_decl))
{
  struct Declaration *self = O_CAST(_self, Declaration());
  self->class_decl = o_cast(_class_decl, ClassDeclaration());
  O_CALL(self->class_decl, retain);
}

O_IMPLEMENT(Declaration, void, set_include_file, (void *_self, void *_include_file))
{
  struct Declaration *self = O_CAST(_self, Declaration());
  self->include_file = o_cast(_include_file, Token());
  O_CALL(self->include_file, retain);
}

O_OBJECT(Declaration, CompileObject);
O_OBJECT_METHOD(Declaration, ctor);
O_OBJECT_METHOD(Declaration, dtor);
O_OBJECT_METHOD(Declaration, set_scope);
O_OBJECT_METHOD(Declaration, set_class_decl);
O_OBJECT_METHOD(Declaration, set_include_file);
O_END_OBJECT
