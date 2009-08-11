#include "Declaration.h"
#include "Token.h"

#define O_SUPER CompileObject()

O_IMPLEMENT(Declaration, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct Declaration * self = O_CAST(_self, Declaration());
  self = O_SUPER->ctor(self, app);  
  /* TODO initialize */
  self->name = o_cast(va_arg(*app, struct Token *), Token());
  return self;
}

O_IMPLEMENT(Declaration, void *, dtor, (void *_self), (_self))
{
  struct Declaration *self = O_CAST(_self, Declaration());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(Declaration, void, set_scope, (void *_self, void *_scope), (_self, _scope))
{
  struct Declaration *self = O_CAST(_self, Declaration());
  self->scope = o_cast(_scope, Scope());
}

O_OBJECT(Declaration, CompileObject);
O_OBJECT_METHOD(Declaration, ctor);
O_OBJECT_METHOD(Declaration, dtor);
O_OBJECT_METHOD(Declaration, set_scope);
O_END_OBJECT
