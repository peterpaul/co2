#include "Declaration.h"
#include "Token.h"
#include "Type.h"
#include "ClassDeclaration.h"

#define O_SUPER CompileObject()

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
}

O_OBJECT(Declaration, CompileObject);
O_OBJECT_METHOD(Declaration, ctor);
O_OBJECT_METHOD(Declaration, dtor);
O_OBJECT_METHOD(Declaration, set_scope);
O_OBJECT_METHOD(Declaration, set_class_decl);
O_END_OBJECT
