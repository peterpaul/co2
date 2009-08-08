#include "Scope.h"
#include "Token.h"

#define O_SUPER Hash()

struct Scope * current_scope = NULL;

O_IMPLEMENT(Scope, void *, ctor, (void *_self, va_list *app), (_self, app))
{
  struct Scope * self = O_CAST(_self, Scope());
  self = O_SUPER->ctor(self, app);
  /* TODO initialize */
  self->parent = current_scope;
  current_scope = self;
  return self;
}

O_IMPLEMENT(Scope, void *, dtor, (void *_self), (_self))
{
  struct Scope *self = O_CAST(_self, Scope());
  /* TODO cleanup */
  return O_SUPER->dtor(self);
}

O_IMPLEMENT(Scope, struct Declaration *, declare, (void *_self, struct Declaration * decl), (_self, decl))
{
  struct Scope *self = O_CAST(_self, Scope());
  O_CALL(self, add, decl->name->name->data, decl);
  O_CALL(decl, set_scope, self);
  return decl;
}

O_IMPLEMENT(Scope, void, leave, (void *_self), (_self))
{
  struct Scope *self = O_CAST(_self, Scope());
  current_scope = self->parent;
}

O_OBJECT(Scope, Hash);
O_OBJECT_METHOD(Scope, ctor);
O_OBJECT_METHOD(Scope, dtor);
O_OBJECT_METHOD(Scope, declare);
O_OBJECT_METHOD(Scope, leave);
O_END_OBJECT
